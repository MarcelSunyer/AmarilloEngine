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
	std::string libraryfolder = applic->file_system->GetLibraryPrefabPath();

	std::filesystem::path newpath = std::filesystem::path(libraryfolder) / uuids::to_string<wchar_t>(guid);
	newpath.replace_extension(".prefab");

	std::string newpath_s = newpath.string();

	JSON_Doc* prefab = applic->json_module->LoadJSON(newpath_s.c_str());

	ResourcePrefab* ret = new ResourcePrefab(guid, prefab);

	return ret;
}

