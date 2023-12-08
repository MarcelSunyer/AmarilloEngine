#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

#include "Resource.h"
#include "ModuleFileSystem.h"
#include <map>
#include <vector>
#include "../External/MathGeoLib/include/Math/float3.h"
#include "Functions.h"
#include <filesystem>
#include "../External/Guid/uuid.h"


class GameObject;

class ResourceLoader
{
public:
	ResourceLoader(ResourceType _resources_to_load, std::string _library_path, std::string _loader_name);
	virtual ~ResourceLoader();

	// Extensions

	void AddAssetExtensionToLoad(const char* extension);
	void AddLibraryExtensionToLoad(const char* extension);

	bool CanLoadExtensionAsset(const char* extension);
	bool CanLoadExtensionLibrary(const char* extension);

	// LOADER ---------------------------------------------------
	ResourceType GetLoaderType();

	virtual bool CreateLibraryFromAsset(std::filesystem::path path, uuids::uuid uid) = 0;
	virtual Resource* LoadResourceFromLibrary(uuids::uuid guid) = 0;

protected:
	ResourceType resources_to_load = ResourceType::RT_NULL;
	std::string library_path;

	std::vector<std::string> assets_extensions_to_load;
	std::vector<std::string> library_extensions_to_load;

	std::string loader_name;
};

#endif //!__RESOURCE_LOADER_H__