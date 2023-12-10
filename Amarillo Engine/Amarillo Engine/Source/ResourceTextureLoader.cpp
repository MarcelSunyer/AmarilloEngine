
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

//bool ResourceTextureLoader::LoadFileToEngine(DecomposedFilePath d_filepath, std::vector<Resource*>& resources)
//{
//
//}


//bool ResourceTextureLoader::RemoveAssetInfoFromEngine(DecomposedFilePath d_filepath)
//{
//	bool ret = false;
//
//	std::string meta_path = d_filepath.file_path + ".meta";
//
//	JSON_Doc* doc = applic->json_module->LoadJSON(meta_path.c_str());
//
//	if (doc != nullptr)
//	{
//		std::string uid = doc->GetString("resource");
//
//		Resource* res = applic->resourceManager->Get(uid);
//
//		if (res != nullptr)
//		{
//			applic->resourceManager->ClearResourceFromGameObjects(res);
//		}
//
//		applic->resourceManager->DeleteResource(uid);
//
//		std::string resource_path = applic->file_system->GetLibraryTexturePath() + uid + ".dds";
//		std::string meta_path = applic->file_system->GetLibraryTexturePath() + uid + ".meta";
//
//		applic->file_system->FileDelete(resource_path.c_str());
//		applic->file_system->FileDelete(meta_path.c_str());
//
//		applic->json_module->UnloadJSON(doc);
//	}
//
//	return ret;
//}

//bool ResourceTextureLoader::ExportResourceToLibrary(Resource* resource)
//{
//	bool ret = false;
//
//	if (resource != nullptr)
//	{
//		ResourceTexture* resource_txt = (ResourceTexture*)resource;
//
//		std::string uid = resource->GetUniqueId();
//
//		// -------------------------------------
//		// FILE --------------------------------
//		// -------------------------------------
//		uint size = 0;
//		byte* data = nullptr;
//
//		// To pick a specific DXT compression use
//		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
//
//		// Get the size of the data buffer
//		size = ilSaveL(IL_DDS, NULL, 0);
//
//		if (size > 0)
//		{
//			ilEnable(IL_FILE_OVERWRITE);
//
//			// Allocate data buffer
//			data = new byte[size];
//
//			// Save to buffer with the ilSaveIL function
//			if (ilSaveL(IL_DDS, data, size) > 0)
//				ret = applic->file_system->FileSave(applic->file_system->GetLibraryTexturePath().c_str(), (char*)data, uid.c_str(), "dds", size);
//
//			RELEASE_ARRAY(data);
//
//			ret = true;
//		}
//
//		// -------------------------------------
//		// META --------------------------------
//		// -------------------------------------
//		std::string meta_path = applic->file_system->GetLibraryTexturePath() + uid + ".meta";
//
//		if (applic->file_system->FileExists(meta_path.c_str()))
//			applic->file_system->FileDelete(meta_path.c_str());
//
//		JSON_Doc* doc = applic->json_module->CreateJSON(meta_path.c_str());
//
//		if (doc != nullptr)
//		{
//			doc->SetString("uid", resource_txt->GetUniqueId().c_str());
//			doc->SetString("name", resource_txt->GetFileName().c_str());
//			doc->SetBool("flipped", resource_txt->GetFlipped());
//
//			doc->Save();
//
//			applic->json_module->UnloadJSON(doc);
//		}
//	}
//
//	return ret;
//}

//bool ResourceTextureLoader::ImportResourceFromLibrary(DecomposedFilePath d_filepath)
//{
//	bool ret = false;
//
//	// -------------------------------------
//	// META --------------------------------
//	// -------------------------------------
//	std::string meta_path = d_filepath.path + d_filepath.file_name + ".meta";
//
//	JSON_Doc* doc = applic->json_module->LoadJSON(meta_path.c_str());
//	if (doc != nullptr)
//	{
//		std::string uid = doc->GetString("uid", "no_uid");
//		std::string resource_name = doc->GetString("name");
//		bool flipped = doc->GetBool("flipped");
//
//		// -------------------------------------
//		// FILE --------------------------------
//		// -------------------------------------
//		// Load texture
//
//		ret = ilLoad(IL_TYPE_UNKNOWN, d_filepath.file_path.c_str());
//
//		if (ret)
//		{
//			// Get texture info
//			ILinfo ImageInfo;
//			iluGetImageInfo(&ImageInfo);
//
//			if (!flipped)
//			{
//				iluFlipImage();
//			}
//
//			// Convert image to rgb and a byte chain
//			ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
//
//			ILubyte* data = ilGetData();
//			uint data_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
//			uint image_width = ilGetInteger(IL_IMAGE_WIDTH);
//			uint image_height = ilGetInteger(IL_IMAGE_HEIGHT);
//			uint format = ilGetInteger(IL_IMAGE_FORMAT);
//
//			if (data_size > 0)
//			{
//				// Create texture
//				ResourceTexture* ret = (ResourceTexture*)applic->resourceManager->CreateNewResource(RT_TEXTURE, uid.c_str());
//
//				ret->SetData(data, data_size, image_width, image_height, format, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
//				ret->SetFlipped(flipped);
//				ret->SetFileName(resource_name.c_str());
//			}
//
//			ilDeleteImages(1, &ImageInfo.Id);
//
//			applic->json_module->UnloadJSON(doc);
//		}
//		else
//		{
//			LOG("Cannot load image %s. Error: %s", d_filepath.file_path.c_str(), iluErrorString(ilGetError()));
//		}
//	}
//
//	return ret;
//}
