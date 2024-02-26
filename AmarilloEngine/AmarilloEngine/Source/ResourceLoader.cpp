#include "ResourceLoader.h"

ResourceLoader::ResourceLoader(ResourceType _resources_to_load, std::string _library_path, std::string _loader_name)
{
	resources_to_load = _resources_to_load;
	library_path = _library_path;
	loader_name = _loader_name;
}

ResourceLoader::~ResourceLoader()
{
}


void ResourceLoader::AddAssetExtensionToLoad(const char* extension)
{
	assets_extensions_to_load.push_back(extension);
}

void ResourceLoader::AddLibraryExtensionToLoad(const char* extension)
{
	library_extensions_to_load.push_back(extension);
}

bool ResourceLoader::CanLoadExtensionAsset(const char* extension)
{
	bool ret = false;

	std::string to_check = ToLowerCase(extension);

	for (std::vector<std::string>::iterator it = assets_extensions_to_load.begin(); it != assets_extensions_to_load.end(); ++it)
	{
		if (TextCmp(to_check.c_str(), (*it).c_str()))
		{
			ret = true;
			break;
		}
	}

	return ret;
}

bool ResourceLoader::CanLoadExtensionLibrary(const char* extension)
{
	bool ret = false;

	std::string to_check = ToLowerCase(extension);

	for (std::vector<std::string>::iterator it = library_extensions_to_load.begin(); it != library_extensions_to_load.end(); ++it)
	{
		if (TextCmp(to_check.c_str(), (*it).c_str()))
		{
			ret = true;
			break;
		}
	}

	return ret;
}

ResourceType ResourceLoader::GetLoaderType()
{
	return resources_to_load;
}