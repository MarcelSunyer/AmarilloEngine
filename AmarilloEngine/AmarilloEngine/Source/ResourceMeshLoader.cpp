#include "ResourceMeshLoader.h"
#include "Application.h"
#include "ResourceMesh.h"
#include "ModuleJSON.h"
#include "../External/Assimp/include/postprocess.h"
#include "../External/Assimp\include\cfileio.h"
#include "../External/Assimp\include\cimport.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "ResourceTexture.h"
#include "ComponentTransform.h"
#include "../External/MathGeoLib/include/Math/Quat.h"
#include "../External/MathGeoLib/include/Math/float3.h"


ResourceMeshLoader::ResourceMeshLoader() :
	ResourceLoader(ResourceType::RT_MESH, applic->file_system->GetLibraryMeshPath(), "MeshLoader")
{

}

ResourceMeshLoader::~ResourceMeshLoader()
{
}

bool ResourceMeshLoader::CreateLibraryFromAsset(std::filesystem::path path, uuids::uuid uid)
{
	std::string path_c = path.string();

	const aiScene* imported_scene = aiImportFile(path_c.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (imported_scene == nullptr)
	{
		return false;
	}

	if (!imported_scene->HasMeshes())
	{
		return false;
	}

	aiNode* root = imported_scene->mRootNode;
	
	std::vector<char> stream;

	AddNodeToBinaryStream(imported_scene, root, stream);

	std::string libraryfolder = applic->file_system->GetLibraryMeshPath();

	std::filesystem::path newpath = std::filesystem::path(libraryfolder) / uuids::to_string<wchar_t>(uid);
	newpath.replace_extension(".amarillomesh");

	applic->file_system->FileSave(
		newpath.string().c_str(),
		(char*)&stream[0],
		stream.size()
	);

	return true;
}

Resource* ResourceMeshLoader::LoadResourceFromLibrary(uuids::uuid guid)
{
	std::string libraryfolder = applic->file_system->GetLibraryMeshPath();

	std::filesystem::path newpath = std::filesystem::path(libraryfolder) / uuids::to_string<wchar_t>(guid);
	newpath.replace_extension(".amarillomesh");

	std::vector<char> file = applic->file_system->LoadFile(newpath);

	std::vector<ChildMesh*> rootMeshes;

	char* cursor = &file[0];

	GetMeshesFromBinaryStream(cursor, rootMeshes);

	ResourceMesh* mesh = new ResourceMesh(guid);

	mesh->childs = rootMeshes;

	return mesh;
}

void ResourceMeshLoader::AddNodeToBinaryStream(const aiScene* scene, aiNode* node, std::vector<char>& stream)
{
	uint size = sizeof(uint[2]);

	//Casa duplicada al exportar el motor y se va de memoria por eso me peta

	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		int numIndices = 0;

		for (uint y = 0; y < mesh->mNumFaces; ++y)
		{
			aiFace face = mesh->mFaces[y];

			if (face.mNumIndices != 3)
			{
				continue;
			}

			numIndices += face.mNumIndices;
		}

		size += sizeof(uint[3]);

		size += mesh->mNumVertices * 3 * sizeof(float);
		size += numIndices * sizeof(uint);

		if (mesh->mTextureCoords != nullptr) 
		{
			size += mesh->mNumVertices * 3 * sizeof(float);
		}
	}

	char* data = new char[size];
	char* cursor = data;

	// Store ranges
	uint nodeRanges[2] = { (uint)node->mNumMeshes, (uint)node->mNumChildren };
	uint bytes = sizeof(nodeRanges);
	memcpy(cursor, nodeRanges, bytes);
	cursor += bytes;

	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		int numIndices = 0;

		for (uint y = 0; y < mesh->mNumFaces; ++y)
		{
			aiFace face = mesh->mFaces[y];

			if (face.mNumIndices != 3)
			{
				continue;
			}

			numIndices += face.mNumIndices;
		}

		int numUvs = mesh->mTextureCoords != nullptr ? mesh->mNumVertices : 0;

		uint meshRanges[3] = { mesh->mNumVertices, numIndices, numUvs };
		uint bytes = sizeof(meshRanges);
		memcpy(cursor, meshRanges, bytes);
		cursor += bytes;

		// Store vertices
		bytes = sizeof(float) * mesh->mNumVertices * 3;
		memcpy(cursor, mesh->mVertices, bytes);
		cursor += bytes;

		for (uint y = 0; y < mesh->mNumFaces; ++y)
		{
			aiFace face = mesh->mFaces[y];

			if (face.mNumIndices != 3)
			{
				continue;
			}

			bytes = 3 * sizeof(int);
			memcpy(cursor, face.mIndices, bytes);
			cursor += bytes;
		}

		// Store uvs
		bytes = sizeof(float) * numUvs * 3;
		memcpy(cursor, (float*)mesh->mTextureCoords[0], bytes);
		cursor += bytes;
	}

	stream.insert(stream.end(), data, data + size);

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		aiNode* child = node->mChildren[i];

		AddNodeToBinaryStream(scene, child, stream);
	}
}

void ResourceMeshLoader::GetMeshesFromBinaryStream(char* cursor, std::vector<ChildMesh*>& meshes)
{
	uint nodeRanges[2];
	uint bytes = sizeof(nodeRanges);
	if (cursor == nullptr)
	{
		return;
	}
	memcpy(nodeRanges, cursor, bytes);
	cursor += bytes;

	

	for (unsigned int i = 0; i < nodeRanges[0]; ++i)
	{
		ChildMesh* mesh = new ChildMesh();

		uint meshRanges[3];
		uint bytes = sizeof(meshRanges);
		memcpy(meshRanges, cursor, bytes);
		cursor += bytes;

		uint verticesCount = meshRanges[0];
		uint verticesSize = verticesCount * 3;
		bytes = sizeof(float) * verticesSize;
		float* vertices = new float[verticesSize];
		memcpy(vertices, cursor, bytes);
		cursor += bytes;

		uint indicesCount = meshRanges[1];
		uint indicesSize = indicesCount;
		bytes = sizeof(uint) * indicesSize;
		uint* indices = new uint[indicesSize];
		memcpy(indices, cursor, bytes);
		cursor += bytes;

		uint uvsCount = meshRanges[2];
		uint uvsSize = uvsCount * 3;
		bytes = sizeof(float) * uvsSize;
		float* uvs = new float[uvsSize];
		memcpy(uvs, cursor, bytes);
		cursor += bytes;

		mesh->SetFaces(vertices, verticesCount, indices, indicesCount);
		mesh->SetUvs(uvs, uvsCount);

		meshes.push_back(mesh);
		
	}

	for (unsigned int i = 0; i < nodeRanges[1]; ++i)
	{
		GetMeshesFromBinaryStream(cursor, meshes);
	}
}