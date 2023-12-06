#include "Resource_Loader.h"

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

}

void ResourceLoader::AddLibraryExtensionToLoad(const char* extension)
{

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

Resource* ResourceLoader::GetResource(std::string unique_id)
{
	std::map<std::string, Resource*>::iterator it = resources.find(unique_id);

	if (it != resources.end())
		return it->second;

	return nullptr;
}

void ResourceLoader::AddResource(Resource* resource)
{
	resources[resource->GetUniqueId()] = resource;
}

bool ResourceLoader::DeleteResource(std::string unique_id)
{
	bool ret = false;

	std::map<std::string, Resource*>::iterator it = resources.find(unique_id);

	if (it != resources.end())
	{
		it->second->CleanUp();
		RELEASE(it->second);
		resources.erase(it);

		ret = true;
	}

	return ret;
}

std::map<std::string, Resource*> ResourceLoader::GetResources()
{
	return resources;
}

ResourceType ResourceLoader::GetLoaderType()
{
	return resources_to_load;
}

float3 ResourceLoader::GetResourceColour() const
{
	return resource_colour;
}