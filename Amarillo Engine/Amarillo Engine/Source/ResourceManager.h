#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__
#include "Application.h"
#include "Module.h"
#include "Globals.h"
#include "Random.h"
#include <map>
#include "Random.h"
#include "Resource.h"
#include "ResourceLoader.h"
#include "../External/Guid/uuid.h"


class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	virtual ~ModuleResourceManager();

	bool Awake();
	bool Start();
	bool CleanUp();

	void OnStartEngine();
	void OnLoadFile(const char* filepath);

	uuids::uuid NewGuid();

	ResourceLoader* GetLoader(ResourceType type);
	std::vector<ResourceLoader*> GetLoaders() const;
	ResourceType GetAssetsResourceTypeFromExtension(const char* extension);
	ResourceType GetLibraryResourceTypeFromExtension(const char* extension);

	void LoadNewFile(std::string path);
	void CreateMetaForAsset(std::filesystem::path assetpath, uuids::uuid uid);
	void LoadResourcesFromAssets();
	
	// Resource management
	Resource* Get(std::string unique_id);
	bool DeleteResource(std::string unique_id);

	std::map<ResourceType, std::vector<Resource*>> GetResources();

	// Meta control
	bool IsAssetMeta(const char* filepath, const char* metapath);
	bool FileIsMeta(const char* filepath);

private:
	void AddLoader(ResourceLoader* loader);

private:
	std::vector<ResourceLoader*> loaders;
	std::map<ResourceType, std::vector<Resource*>> resourcesLoaded;

	bool draw_resource_selector_win = false;
};
#endif //__RESOURCE_MANAGER_H__
