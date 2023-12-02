#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Application.h"
#include "Module.h"

#include "Define_Resource.h"
#include "AssetDirectory.h"

#include <map>
#include "Random.h"


class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	virtual ~ModuleResourceManager();
	
	bool Init() override;
	bool Start() override;

	update_status PreUpdate(float dt) override;

	bool CleanUp() override;

	void GetUID();
	int ExistsOnLibrary(const char* file_in_assets) const;

	int ImportFile(const char* assetsFile, Resource::Type type);

	int CreateLibraryFromAssets(const char* assetsFile);
	void RequestFromAssets(const char* assets_path);

	int GenerateNewUID();

	Resource* CreateNewResource(const char* assetsFile, uint uid, Resource::Type type);
	Resource* LoadFromLibrary(const char* libraryFile, Resource::Type type, uint _uid);

	Resource* RequestResource(int uid, const char* libraryPath = nullptr);
	void UnloadResource(int uid);

	Resource::Type GetTypeFromAssetExtension(const char* assetFile);
	Resource::Type GetTypeFromLibraryExtension(const char* libraryFile);

	void GenerateMeta(const char* aPath, const char* lPath, unsigned int uid, Resource::Type type);
	std::string LibraryFromMeta(const char* metaFile);
	std::string GenLibraryPath(uint _uid, Resource::Type _type);

	int GetMetaUID(const char* metaFile);
	std::string GetMetaPath(const char* assetsFile);
	Resource::Type GetMetaType(const char* metaFile);

private:
	void LoadResource(int uid);
	void ReleaseResource(int uid);

	bool IsResourceLoaded(int uid);
	void UpdateFile(AssetDir& modDir);


private:
	std::map<int, Resource*> resources;

	float fileCheckTime;
	float fileUpdateDelay;

public:
	/*AssetDir assetsRoot;
	AssetDir meshesLibraryRoot;*/
};
#endif //__RESOURCE_H__
