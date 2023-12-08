#ifndef __RESOURCE_TEXTURE_LOADER_H__
#define __RESOURCE_TEXTURE_LOADER_H__

#include "ResourceTexture.h"
#include "Resource.h"
#include "ResourceLoader.h"
#include <vector>

class ResourceTexture;

struct TextureInfo
{
	TextureInfo() {};
	TextureInfo(const TextureInfo& copy) {
		id = copy.id; size_x = copy.size_x; size_y = copy.size_y;
	};
	TextureInfo(unsigned int _id, unsigned int _size_x, unsigned int _size_y) { id = _id; size_x = _size_x; size_y = _size_y; };

	unsigned int id = 0;
	unsigned int size_x = 0;
	unsigned int size_y = 0;
};

class ResourceTextureLoader : public ResourceLoader
{
public:
	ResourceTextureLoader();
	virtual ~ResourceTextureLoader();

	uuids::uuid CreateLibraryFromAsset(std::filesystem::path path) override;
	Resource* LoadResourceFromLibrary(uuids::uuid guid) override;
};

#endif