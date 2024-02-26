#include "ModuleMesh.h"
#include "Globals.h"
#include "Application.h"
#include "../External/Assimp/include/cimport.h"
#include "../External/Assimp/include/scene.h"
#include "../External/Assimp/include/postprocess.h"
#include <vector>
#include "GameObject.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "../External/MathGeoLib/include/Math/float3.h"
#include "GameObject.h"
#include "../External/Assimp/include/cimport.h"
#include "../External/Assimp/include/scene.h"
#pragma comment (lib, "External/Assimp/libx86/assimp.lib")

ModuleMesh::ModuleMesh(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleMesh::~ModuleMesh()
{
}

bool ModuleMesh::Init()
{
	return true;
}

update_status ModuleMesh::Update(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleMesh::CleanUp()
{
	return true;
}

GameObject* ModuleMesh::LoadMesh(const char* file_path)
{
	const aiScene* imported_scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (imported_scene->HasMeshes() && imported_scene != nullptr)
	{
		GameObject* parentObject = App->scene->CreateGameObject("GameObject", App->scene->root_object);

		GetSceneInfo(imported_scene->mRootNode, imported_scene, file_path, parentObject);

		aiReleaseImport(imported_scene);

		return parentObject;
	}

	LOG("Error loading scene % s", file_path);

	return nullptr;
}

void ModuleMesh::DrawNormals() {
	float length = 0.2f;

	glBegin(GL_LINES);

	for (Mesh* mesh : ourMeshes) {
		for (unsigned int i = 0; i < mesh->indices.size(); i += 3) {

			float3 vertex1 = mesh->ourVertex[mesh->indices[i]].Position;
			float3 vertex2 = mesh->ourVertex[mesh->indices[i + 1]].Position;
			float3 vertex3 = mesh->ourVertex[mesh->indices[i + 2]].Position;

			float3 center = (vertex1 + vertex2 + vertex3) / 3.0f;


			float3 normal = mesh->ourVertex[mesh->indices[i]].Normal;


			float3 normalEnd = center + normal * length;


			glColor3f(1.0f, 1.0f, 0.0f); 

			glVertex3f(center.x, center.y, center.z);
			glVertex3f(normalEnd.x, normalEnd.y, normalEnd.z);
		}
	}

	glEnd();
}

void ModuleMesh::GetSceneInfo(aiNode* node, const aiScene* scene, const char* file_path, GameObject* gameObject)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		ProcessMesh(scene->mMeshes[node->mMeshes[i]], node, scene, file_path, gameObject);

	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		GetSceneInfo(node->mChildren[i], scene, file_path, gameObject);
	}
}

ModuleMesh::Mesh ModuleMesh::ProcessMesh(aiMesh* mesh, aiNode* node, const aiScene* scene, const char* file_path, GameObject* gameObject)
{
	GameObject* newMesh = App->scene->CreateGameObject(node->mName.C_Str(), gameObject);

	Mesh* myMesh = new Mesh();

	for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
	{
		Vertex vertex;
		float3 vector;
		vector.x = mesh->mVertices[j].x;
		vector.y = mesh->mVertices[j].y;
		vector.z = mesh->mVertices[j].z;
		vertex.Position = vector;

		if (mesh->HasNormals())
		{
			vertex.Normal.x = mesh->mNormals[j].x;
			vertex.Normal.y = mesh->mNormals[j].y;
			vertex.Normal.z = mesh->mNormals[j].z;

		}
		if (mesh->HasTextureCoords(0))
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][j].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][j].y;
		}
		else
		{
			vertex.TexCoords.x = 0.0f;
			vertex.TexCoords.y = 0.0f;
		}

		myMesh->ourVertex.push_back(vertex);
	}

	if (mesh->HasFaces())
	{
		myMesh->indices.resize(mesh->mNumFaces * 3);

		for (uint y = 0; y < mesh->mNumFaces; ++y)
		{
			if (mesh->mFaces[y].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&myMesh->indices[y * 3], mesh->mFaces[y].mIndices, 3 * sizeof(unsigned int));
			}
		}
	}

	// Process Textures
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);

		if (numTextures > 0) 
		{
			aiString aiPath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath);

			std::string path = std::string(aiPath.C_Str());
			std::string asset_dir = "../Assets/Street_Environment/";	


			Texture tmpTexture;


			tmpTexture.path_ = asset_dir + path;


			ComponentTexture* text_component = (ComponentTexture*)newMesh->AddComponent(ComponentTypes::TEXTURE);
			
			text_component->SetTexture(&tmpTexture);
			LOG("File TEXTURE Path: %s", tmpTexture.path_.c_str());
			
			
			App->texture->LoadTextureToGameObject(newMesh, tmpTexture.path_);

			myMesh->ourTextures.push_back(&tmpTexture);
			
		}
	}

	ComponentMesh* mesh_component = (ComponentMesh*)newMesh->AddComponent(ComponentTypes::MESH);
	mesh_component->SetMesh(myMesh);
	mesh_component->SetPath(file_path);
	LOG("File MESH Path: %s", file_path);
	

	ourMeshes.push_back(myMesh);

	App->renderer3D->BindBuffers();
	

	return *myMesh;
}

std::string ModuleMesh::ObtainFileName(const std::string& ruta) {
	
	size_t ultimaBarra = ruta.find_last_of("\\/");

	if (ultimaBarra != std::string::npos) {
		return ruta.substr(ultimaBarra + 1);
	}
	else {
		return ruta;
	}
}