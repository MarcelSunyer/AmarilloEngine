#ifndef __RESOURCE_PREFAB_LOADER_H__
#define __RESOURCE_PREFAB_LOADER_H__

#include "ResourceLoader.h"
#include "../External/MathGeoLib/include/Geometry/AABB.h"
#include "../External/Assimp/include/scene.h"
#include "ResourcePrefab.h"
#include "ResourceMesh.h"

class ResourcePrefabLoader : public ResourceLoader
{
public:
	ResourcePrefabLoader();
	virtual ~ResourcePrefabLoader();

	bool CreateLibraryFromAsset(std::filesystem::path path, uuids::uuid uid) override;
	Resource* LoadResourceFromLibrary(uuids::uuid guid) override;
};
#endif