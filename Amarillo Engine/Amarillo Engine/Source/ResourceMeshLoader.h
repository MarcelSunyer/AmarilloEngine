#ifndef __RESOURCE_MESH_LOADER_H__
#define __RESOURCE_MESH_LOADER_H__

#include "ResourceLoader.h"
#include "../External/MathGeoLib/include/Geometry/AABB.h"
#include "../External/Assimp/include/scene.h"
#include "ResourceMesh.h"

class ResourceMeshLoader : public ResourceLoader
{
public:
	ResourceMeshLoader();
	virtual ~ResourceMeshLoader();

	bool CreateLibraryFromAsset(std::filesystem::path path, uuids::uuid uid) override;
	Resource* LoadResourceFromLibrary(uuids::uuid guid) override;
private:
	void AddNodeToBinaryStream(const aiScene* scene, aiNode* node, std::vector<char>& stream);
	void GetMeshesFromBinaryStream(char* cursor, std::vector<ChildMesh*>& meshes);
};

#endif