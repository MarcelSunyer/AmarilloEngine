#include "ResourcePrefabLoader.h"

ResourcePrefabLoader::ResourcePrefabLoader() : ResourceLoader(ResourceType::RT_PREFAB, applic->file_system->GetLibraryMeshPath(), "MeshLoader")
{
}

ResourcePrefabLoader::~ResourcePrefabLoader()
{
}

bool ResourcePrefabLoader::CreateLibraryFromAsset(std::filesystem::path path, uuids::uuid uid)
{
	return false;
}

Resource* ResourcePrefabLoader::LoadResourceFromLibrary(uuids::uuid guid)
{
	return nullptr;
}

void ResourcePrefabLoader::AddNodeToBinaryStream(const aiScene* scene, aiNode* node, std::vector<char>& stream)
{
}

void ResourcePrefabLoader::GetPrefabFromBinaryStream(char* cursor, std::vector<ChildMesh*>& meshes)
{

}
