#include "Resource_Mesh_Loader.h"
#include "Application.h"
#include "ResourceMesh.h"
#include "ModuleJSON.h"
#include "../External/Assimp/include/postprocess.h"
#include "../External/Assimp\include\cfileio.h"
#include "../External/Assimp\include\cimport.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "ResourceTexture.h"

ResourceMeshLoader::ResourceMeshLoader() :
	ResourceLoader(ResourceType::RT_MESH, applic->file_system->GetLibraryMeshPath(), "MeshLoader")
{

}

ResourceMeshLoader::~ResourceMeshLoader()
{
}

Resource* ResourceMeshLoader::CreateResource(std::string new_uid)
{
	Resource* ret = nullptr;

	ret = new ResourceMesh(new_uid);

	return ret;
}

bool ResourceMeshLoader::LoadFileToEngine(DecomposedFilePath d_filepath, std::vector<Resource*>& resources)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(d_filepath.file_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene == nullptr)
		ret = false;

	if (ret && !scene->HasMeshes())
	{
		LOG("WARNING, scene has no meshes! Path: %s", d_filepath.file_path.c_str());
		ret = false;
	}

	if (ret)
	{
		aiNode* root = scene->mRootNode;

		// Root transform
		float3 position(0, 0, 0);
		Quat rotation(0, 0, 0, 0);
		float3 scale(0, 0, 0);

		aiVector3D aitranslation;
		aiVector3D aiscaling;
		aiQuaternion airotation;

		if (root != nullptr)
		{
			root->mTransformation.Decompose(aiscaling, airotation, aitranslation);
			position = float3(aitranslation.x, aitranslation.y, aitranslation.z);
			scale = float3(aiscaling.x, aiscaling.y, aiscaling.z);
			rotation = Quat(airotation.x, airotation.y, airotation.z, airotation.w);
		}

		// Create root go
		GameObject* parent = new GameObject(" ");

		parent->transform->SetLocalPosition(float3(position.x, position.y, position.z));
		parent->transform->SetLocalRotation(Quat(rotation.x, rotation.y, rotation.w, rotation.z));
		parent->transform->SetLocalScale(float3(scale.x, scale.y, scale.z));

		std::string name = applic->file_system->GetFileNameFromFilePath(d_filepath.file_path.c_str());
		name = applic->file_system->GetFilenameWithoutExtension(name.c_str());
		parent->mName = d_filepath.file_name.c_str();

		// Total mesh bbox
		AABB total_abb;
		total_abb.SetNegativeInfinity();

		// Keep track of resources loaded (avoid repeating)

		// Iterate
		for (int i = 0; i < root->mNumChildren; i++)
		{
			RecursiveLoadMesh(scene, root->mChildren[i], d_filepath.file_path.c_str(), total_abb, resources, parent);
		}

		used_resources.clear();

		// Set camera focus
		applic->camera->GetEditorCamera()->Focus(total_abb);

		// Crate meta
		std::string meta_path = (applic->file_system->GetAssetsPath().c_str() + d_filepath.file_name + "." + d_filepath.file_extension + ".meta");

		if (applic->file_system->FileExists(meta_path.c_str()))
			applic->file_system->FileDelete(meta_path.c_str());

		JSON_Doc* doc = applic->json_module->CreateJSON(meta_path.c_str());
		if (doc != nullptr)
		{
			doc->SetArray("resources");

			for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
			{
				doc->AddStringToArray("resources", (*it)->GetUniqueId().c_str());
			}

			doc->Save();
			applic->json_module->UnloadJSON(doc);
		}

		applic->scene->DeleteGameObject(parent);
	}

	return ret;
}


bool ResourceMeshLoader::RemoveAssetInfoFromEngine(DecomposedFilePath d_filepath)
{
	bool ret = false;

	std::string meta_file = d_filepath.file_path + ".meta";

	JSON_Doc* doc = applic->json_module->LoadJSON(meta_file.c_str());

	if (doc != nullptr)
	{
		std::string prefab_uid = doc->GetString("meshprefab_uid");

		applic->resourceManager->DeleteResource(prefab_uid);

		std::string prefab_path = applic->file_system->GetLibraryMeshPath() + prefab_uid + ".meshprefab";
		applic->file_system->FileDelete(prefab_path.c_str());

		applic->resourceManager->DeleteResource(prefab_uid);

		int resources_count = doc->GetArrayCount("resources");;
		for (int i = 0; i < resources_count; ++i)
		{
			std::string uid = doc->GetStringFromArray("resources", i);

			Resource* res = applic->resourceManager->Get(uid);

			if (res != nullptr)
			{
				applic->resourceManager->ClearResourceFromGameObjects(res);
			}

			applic->resourceManager->DeleteResource(uid);

			std::string resource_path = applic->file_system->GetLibraryMeshPath() + uid + ".sustomesh";
			std::string meta_path = applic->file_system->GetLibraryMeshPath() + uid + ".meta";

			applic->file_system->FileDelete(resource_path.c_str());
			applic->file_system->FileDelete(meta_path.c_str());

			ret = true;
		}

		applic->json_module->UnloadJSON(doc);
	}

	return ret;
}

void ResourceMeshLoader::ClearFromGameObject(Resource* resource, GameObject* go)
{
	if (go != nullptr && resource != nullptr)
	{
		ComponentMesh* mesh = (ComponentMesh*)go->GetComponent(ComponentTypes::MESH);

		if (mesh != nullptr)
		{
			/*if (mesh->GetMesh() != nullptr)
			{
				if (mesh->GetMesh()->GetUniqueId() == resource->GetUniqueId())
					mesh->RemoveMesh();
			}*/
		}
	}
}


bool ResourceMeshLoader::ExportResourceToLibrary(Resource* resource)
{
	bool ret = true;

	if (resource != nullptr)
	{
		ResourceMesh* mesh = (ResourceMesh*)resource;

		std::string name = mesh->GetUniqueId();

		// -------------------------------------
		// FILE --------------------------------
		// -------------------------------------
		uint ranges[3] = { mesh->GetNumVertices(), mesh->GetNumIndices(), mesh->GetNumUVs() };
		uint size = sizeof(ranges) +
			sizeof(uint) * mesh->GetNumIndices() +
			sizeof(float) * mesh->GetNumVertices() * 3 +
			sizeof(float) * mesh->GetNumUVs() * 3;

		// Allocate data
		char* data = new char[size];
		char* cursor = data;

		// Store ranges
		uint bytes = sizeof(ranges);
		memcpy(cursor, ranges, bytes);
		cursor += bytes;

		// Store indices
		bytes = sizeof(uint) * mesh->GetNumIndices();
		memcpy(cursor, mesh->GetIndices(), bytes);
		cursor += bytes;

		// Store vertices
		bytes = sizeof(float) * mesh->GetNumVertices() * 3;
		memcpy(cursor, mesh->GetVertices(), bytes);
		cursor += bytes;

		// Store UVs
		bytes = sizeof(float) * mesh->GetNumUVs() * 3;
		memcpy(cursor, mesh->GetUVs(), bytes);

		//fopen
		if (applic->file_system->FileSave(library_path.c_str(), data, name.c_str(), "sustomesh", size) == false)
		{
			RELEASE_ARRAY(data);
			return false;
		}

		RELEASE_ARRAY(data);

		// -------------------------------------
		// META --------------------------------
		// -------------------------------------
		std::string meta_name = library_path + name + ".meta";

		if (applic->file_system->FileExists(meta_name.c_str()))
			applic->file_system->FileDelete(meta_name.c_str());

		JSON_Doc* doc = applic->json_module->CreateJSON(meta_name.c_str());

		if (doc != nullptr)
		{
			doc->Clear();

			doc->SetString("uid", mesh->GetUniqueId().c_str());
			doc->SetString("name", mesh->GetFileName().c_str());

			doc->Save();

			applic->json_module->UnloadJSON(doc);
		}
	}

	return ret;
}


bool ResourceMeshLoader::ImportResourceFromLibrary(DecomposedFilePath d_filepath)
{
	bool ret = true;

	// -------------------------------------
	// META --------------------------------
	// -------------------------------------
	std::string meta_name = d_filepath.path + d_filepath.file_name + ".meta";
	JSON_Doc* doc = applic->json_module->LoadJSON(meta_name.c_str());
	if (doc != nullptr)
	{
		std::string uid = doc->GetString("uid", "no_uid");
		std::string resource_name = doc->GetString("name");

		// -------------------------------------
		// FILE --------------------------------
		// -------------------------------------
		//Open the file and get the size
		FILE* file = fopen(d_filepath.file_path.c_str(), "rb");

		if (file != nullptr)
		{
			fseek(file, 0, SEEK_END);
			uint size = ftell(file);
			rewind(file);

			// Create a buffer to get the data of the file
			char* buffer = new char[size];
			char* cursor = buffer;

			// Read the file and close it
			fread(buffer, sizeof(char), size, file);
			fclose(file);

			// Copy the ranges
			// ranges[0] = Vertices, ranges[1] = Indices, ranges[2] = Uvs
			uint ranges[3];
			uint bytes = sizeof(ranges);
			memcpy(ranges, cursor, bytes);
			cursor += bytes;

			// Store indices
			uint* indices = new uint[ranges[1]];
			bytes = sizeof(uint) * ranges[1];
			memcpy(indices, cursor, bytes);
			cursor += bytes;

			// Store vertices
			float* vertices = new float[ranges[0] * 3];
			bytes = sizeof(float) * ranges[0] * 3;
			memcpy(vertices, cursor, bytes);
			cursor += bytes;

			// Store UVs
			float* uvs = new float[ranges[2] * 3];
			bytes = sizeof(float) * ranges[2] * 3;
			memcpy(uvs, cursor, bytes);
			cursor += bytes;

			// Create mesh --------------
			ResourceMesh* new_mesh = (ResourceMesh*)applic->resourceManager->CreateNewResource(RT_MESH, uid);
			new_mesh->SetFaces(vertices, ranges[0], indices, ranges[1]);
			new_mesh->SetUvs(uvs, ranges[2]);

			new_mesh->SetFileName(resource_name.c_str());

			RELEASE_ARRAY(buffer);
			RELEASE_ARRAY(indices);
			RELEASE_ARRAY(vertices);
			RELEASE_ARRAY(uvs);

			ret = true;
		}

		applic->json_module->UnloadJSON(doc);
	}
	return ret;
}

bool ResourceMeshLoader::LoadAssetIntoScene(DecomposedFilePath decomposed_file_path)
{
	bool ret = false;

	std::string meta_file = decomposed_file_path.file_path + ".meta";

	JSON_Doc* doc = applic->json_module->LoadJSON(meta_file.c_str());

	if (doc != nullptr)
	{
		std::string prefab_uid = doc->GetString("meshprefab_uid");

		/*GameObjectAbstraction abs = r_prefab->GetAbstraction();

		if (abs.GetValid())
		{
			App->gameobj->GetAbstractor()->DeAbstract(abs);
		}*/
	}

	return ret;
}

bool ResourceMeshLoader::IsAssetOnLibrary(DecomposedFilePath d_filepath, std::vector<std::string>& library_files_used)
{
	bool ret = false;

	int correct = 0;
	int resources_count = -9999;

	std::string meta_file = d_filepath.file_path + ".meta";

	if (applic->file_system->FileExists(meta_file.c_str()))
	{
		JSON_Doc* doc = applic->json_module->LoadJSON(meta_file.c_str());
		if (doc != nullptr)
		{
			std::string prefab_uid = doc->GetString("meshprefab_uid");

			std::string prefab_path = library_path + prefab_uid + ".meshprefab";

			library_files_used.push_back(prefab_path);

			if (applic->file_system->FileExists(prefab_path.c_str()))
			{
				++correct;
			}

			resources_count = doc->GetArrayCount("resources");
			for (int i = 0; i < resources_count; ++i)
			{
				std::string uid = doc->GetStringFromArray("resources", i);

				std::string resource_path = library_path + uid + ".sustomesh";
				std::string meta_path = library_path + uid + ".meta";

				library_files_used.push_back(resource_path);
				library_files_used.push_back(meta_path);

				if (applic->file_system->FileExists(resource_path.c_str()) && applic->file_system->FileExists(meta_path.c_str()))
				{
					++correct;
				}
			}
		}
	}

	if (correct == resources_count + 1)
		ret = true;

	return ret;
}


bool ResourceMeshLoader::RenameAsset(DecomposedFilePath d_filepath, const char* new_name)
{
	bool ret = true;

	std::string meta_file = d_filepath.file_path + ".meta";

	if (!applic->file_system->FileRename(d_filepath.file_path.c_str(), new_name))
	{
		ret = false;
	}

	if (!applic->file_system->FileRename(meta_file.c_str(), new_name))
	{
		ret = false;
	}

	return ret;
}

void ResourceMeshLoader::RecursiveLoadMesh(const aiScene* scene, aiNode* node, const char* full_path, AABB& total_abb,
	std::vector<Resource*>& resources, GameObject* parent)
{
	bool node_valid = true;

	if (node->mNumMeshes == 0)
		node_valid = false;

	aiMesh* aimesh = nullptr;
	ResourceMesh* mesh = nullptr;
	GameObject* go = new GameObject(" ");
	std::string name = node->mName.C_Str();

	for (int i = 0; i < node->mNumMeshes; i++)
	{
		bool mesh_valid = true;

		int mesh_index = node->mMeshes[i];
		aimesh = scene->mMeshes[mesh_index];

		// Check if its already loaded
		Resource* res_mesh = nullptr;
		bool mesh_already_loaded = false;
		if (ResourceIsUsed(mesh_index, RT_MESH, res_mesh))
		{
			mesh = (ResourceMesh*)res_mesh;
			mesh_already_loaded = true;
		}

		if (mesh_valid && node_valid && !mesh_already_loaded)
		{
			mesh = (ResourceMesh*)applic->resourceManager->CreateNewResource(RT_MESH);
			mesh->SetFileName(name.c_str());

			if (!aimesh->HasFaces())
			{
				LOG("WARNING, geometry has no faces!");
				mesh_valid = false;
			}
		}

		// VERTICES && INDICES
		if (mesh_valid && node_valid && !mesh_already_loaded)
		{
			float* vertices = new float[aimesh->mNumVertices * 3];
			memcpy(vertices, aimesh->mVertices, sizeof(float) * aimesh->mNumVertices * 3);

			uint* indices = new uint[aimesh->mNumFaces * 3];

			for (uint i = 0; i < aimesh->mNumFaces && mesh_valid; ++i)
			{
				if (aimesh->mFaces[i].mNumIndices == 3)
				{
					memcpy(&indices[i * 3], aimesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
				else
				{
					LOG("WARNING, geometry face with != 3 indices!");
					mesh_valid = false;
				}
			}

			mesh->SetFaces(vertices, aimesh->mNumVertices, indices, aimesh->mNumFaces * 3);

			RELEASE_ARRAY(vertices);
			RELEASE_ARRAY(indices);
		}

		// UVS
		if (mesh_valid && node_valid && !mesh_already_loaded && aimesh->HasTextureCoords(0))
		{
			float* uvs = new float[aimesh->mNumVertices * 3];
			memcpy(uvs, (float*)aimesh->mTextureCoords[0], sizeof(float) * aimesh->mNumVertices * 3);

			mesh->SetUvs(uvs, aimesh->mNumVertices);

			RELEASE_ARRAY(uvs);
		}

		// POSITION, ROTATION AND SCALE
		float3 position = float3::zero;
		Quat rotation = Quat::identity;
		float3 scale = float3::one;

		if (mesh_valid && node_valid)
		{
			aiVector3D aitranslation;
			aiVector3D aiscaling;
			aiQuaternion airotation;

			node->mTransformation.Decompose(aiscaling, airotation, aitranslation);
			position = float3(aitranslation.x, aitranslation.y, aitranslation.z);
			scale = float3(aiscaling.x, aiscaling.y, aiscaling.z);
			rotation = Quat(airotation.x, airotation.y, airotation.z, airotation.w);

			mesh->SetTransform(position, rotation, scale);
		}

		// GENERAL BBOX
		if (mesh_valid && node_valid)
		{
			AABB mesh_with_scale = mesh->GetBBox();
			mesh_with_scale.Scale(position, scale);

			total_abb.Enclose(mesh_with_scale);
		}

		//// MATERIALS
		ResourceTexture* texture = nullptr;

		if (mesh_valid && node_valid)
		{
			// Check if its already loaded
			Resource* res_tex = nullptr;

			bool texture_already_loaded = false;

			if (ResourceIsUsed(aimesh->mMaterialIndex, RT_TEXTURE, res_tex))
			{
				texture = (ResourceTexture*)res_tex;
				texture_already_loaded = true;
			}


			// CREATE GAME OBJECT
			if (mesh_valid && node_valid && parent != nullptr)
			{
				if (name == "")
					name = "no_name";

				go->mName = name;

				go->SetParent(parent);

				go->transform->SetLocalPosition(mesh->GetPosition());
				go->transform->SetLocalRotation(mesh->GetRotation());
				go->transform->SetLocalScale(mesh->GetScale());

				go->AddComponent(MESH);
				ComponentMesh* cmesh = (ComponentMesh*)go->GetComponent(MESH);
				//cmesh->SetMesh(mesh);
			}

			if (mesh_valid && node_valid && !mesh_already_loaded && mesh != nullptr)
			{
				ExportResourceToLibrary(mesh);

				AddResource(mesh_index, RT_MESH, mesh);

				resources.push_back(mesh);
			}
			else if (!mesh_valid && !mesh_already_loaded && mesh != nullptr)
				applic->resourceManager->DeleteResource(mesh->GetUniqueId());
		}

		// Select parent
		GameObject* pare = nullptr;
		if (node_valid && go != nullptr)
			pare = go;
		else
			pare = parent;

		// RECURSE
		for (int i = 0; i < node->mNumChildren; i++)
		{
			RecursiveLoadMesh(scene, node->mChildren[i], full_path, total_abb, resources, pare);
		}
	}
}

bool ResourceMeshLoader::ResourceIsUsed(int index, ResourceType type, Resource*& res)
{
	bool ret = false;

	for (std::vector<UsedResource>::iterator it = used_resources.begin(); it != used_resources.end(); it++)
	{
		if ((*it).GetIndex() == index && (*it).GetType() == type)
		{
			res = (*it).GetResource();
			ret = true;
			break;
		}
	}

	return ret;
}

void ResourceMeshLoader::AddResource(int index, ResourceType type, Resource* res)
{
	UsedResource used(res, index, type);
	used_resources.push_back(used);
}

