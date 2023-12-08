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

	// Resource management
	Resource* Get(std::string unique_id);
	bool DeleteResource(std::string unique_id);

	// Resource, assets and library management
	//void LoadFileToEngine(const char* filepath, std::vector<Resource*>& resources = std::vector<Resource*>());
	//void UnloadAssetFromEngine(const char* filepath);
	//void RemoveAssetInfoFromEngine(const char* filepath);
	//void ClearResourceFromGameObjects(Resource* res);
	//void ExportResourceToLibrary(Resource* resource);
	//void ImportResourceFromLibrary(const char* uid);
	//void LoadAssetIntoScene(const char* filepath);
	//bool IsAssetOnLibrary(const char* filepath, std::vector<std::string>& library_files_used = std::vector<std::string>());
	//void ReimportAssetToEngine(const char* filepath);
	//void RenameAsset(const char* filepath, const char* new_name);

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
