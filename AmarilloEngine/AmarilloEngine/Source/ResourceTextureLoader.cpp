
#include "ResourceTextureLoader.h"
#include "Application.h"
#include "ResourceManager.h"
#include "ModuleJSON.h"
#include "ModuleFileSystem.h"
#include "../External/Guid/uuid.h"

ResourceTextureLoader::ResourceTextureLoader() :
	ResourceLoader(ResourceType::RT_TEXTURE, applic->file_system->GetLibraryTexturePath(), "TexturesLoader")
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

ResourceTextureLoader::~ResourceTextureLoader()
{
}

bool ResourceTextureLoader::CreateLibraryFromAsset(std::filesystem::path path, uuids::uuid uid)
{
	bool ret = false;

    std::string path_c = path.string();

	ret = ilLoad(IL_TYPE_UNKNOWN, path_c.c_str());

	if (!ret)
	{
		LOG("Cannot load image %s. Error: %s", path_c, iluErrorString(ilGetError()));
		return false;
	}

	// Get texture info
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);

	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}

	// Convert image to rgb and a byte chain
	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

	ILubyte* data = ilGetData();
	uint data_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
	uint image_width = ilGetInteger(IL_IMAGE_WIDTH);
	uint image_height = ilGetInteger(IL_IMAGE_HEIGHT);
	uint format = ilGetInteger(IL_IMAGE_FORMAT);

	if (data_size <= 0)
	{
		LOG("Cannot load image %s. Error: %s", path_c, iluErrorString(ilGetError()));
		return false;
	}

	std::string libraryfolder = applic->file_system->GetLibraryTexturePath();

	std::filesystem::path newpath = std::filesystem::path(libraryfolder) / uuids::to_string<wchar_t>(uid);
	newpath.replace_extension(".dds");

	ilEnable(IL_FILE_OVERWRITE);
	
	// Allocate data buffer
	data = new byte[data_size];
	
    // Save to buffer with the ilSaveIL function
	if (ilSaveL(IL_DDS, data, data_size) > 0)
	{
		ret = applic->file_system->FileSave(
			newpath.string().c_str(),
			(char*)data, 
			data_size
		);
	}

    RELEASE_ARRAY(data);

	ilDeleteImages(1, &ImageInfo.Id);

	return true;
}

Resource* ResourceTextureLoader::LoadResourceFromLibrary(uuids::uuid guid)
{
	std::string libraryfolder = applic->file_system->GetLibraryTexturePath();

	std::filesystem::path newpath = std::filesystem::path(libraryfolder) / uuids::to_string<wchar_t>(guid);
	newpath.replace_extension(".dds");

	std::string newpath_s = newpath.string();
	bool loaded = ilLoad(IL_TYPE_UNKNOWN, newpath_s.c_str());

	if (!loaded)
	{
		LOG("Cannot load image %s. Error: %s", newpath_s.c_str(), iluErrorString(ilGetError()));
		return nullptr;
	}
	// Get texture info
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);

	// Convert image to rgb and a byte chain
	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

	char* data = (char*)ilGetData();
	uint data_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
	uint image_width = ilGetInteger(IL_IMAGE_WIDTH);
	uint image_height = ilGetInteger(IL_IMAGE_HEIGHT);
	uint format = ilGetInteger(IL_IMAGE_FORMAT);

	if (data_size <= 0)
	{
		LOG("Cannot load image %s. Error: %s", newpath_s.c_str(), iluErrorString(ilGetError()));
		return nullptr;
	}

	ResourceTexture* ret = new ResourceTexture(guid);

	ret->SetData(data, data_size, image_width, image_height, format, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);

	ilDeleteImages(1, &ImageInfo.Id);

	return ret;
}