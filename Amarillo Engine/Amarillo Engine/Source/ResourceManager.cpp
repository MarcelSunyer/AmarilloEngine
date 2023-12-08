#include "ResourceManager.h"
#include "Application.h"
#include "../External/ImGui/imgui.h"
#include "../External/Parson/parson.h"
#include "ModuleScene.h"
#include "ResourceMeshLoader.h"
#include "ResourceTextureLoader.h"


ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Awake()
{
	return true;
}

bool ModuleResourceManager::Start()
{
	//Meter todos los loaders
	bool ret = true;

	//ResourceMeshLoader* mesh_loader = new ResourceMeshLoader();
	//mesh_loader->AddAssetExtensionToLoad("fbx");
	//mesh_loader->AddLibraryExtensionToLoad("mesh");

	ResourceTextureLoader* texture_loader = new ResourceTextureLoader();
	texture_loader->AddAssetExtensionToLoad(".png");
	texture_loader->AddAssetExtensionToLoad(".tga");
	texture_loader->AddAssetExtensionToLoad(".dds");
	texture_loader->AddLibraryExtensionToLoad(".dds");

	//7AddLoader(mesh_loader);
	AddLoader(texture_loader);

	return ret;
}

bool ModuleResourceManager::CleanUp()
{
	return false;
}

void ModuleResourceManager::OnStartEngine()
{
	//Prioriza diferentes cosas
}

void ModuleResourceManager::OnLoadFile(const char* filepath)
{
	
}

uuids::uuid ModuleResourceManager::NewGuid()
{
	std::random_device rd;
	auto seed_data = std::array<int, std::mt19937::state_size> {};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	std::mt19937 generator(seq);
	uuids::uuid_random_generator gen{ generator };
	return gen();
}

ResourceLoader* ModuleResourceManager::GetLoader(ResourceType type)
{
	ResourceLoader* ret = nullptr;

	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		ResourceLoader* loader = (*it);

		if (loader->GetLoaderType() == type)
		{
			ret = loader;
			break;
		}
	}

	return ret;
}
std::vector<ResourceLoader*> ModuleResourceManager::GetLoaders() const
{
	return loaders;
}

ResourceType ModuleResourceManager::GetAssetsResourceTypeFromExtension(const char* extension)
{
	ResourceType ret = ResourceType::RT_NULL;

	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		if ((*it)->CanLoadExtensionAsset(extension))
		{
			ret = (*it)->GetLoaderType();
			break;
		}
	}

	return ret;
}

ResourceType ModuleResourceManager::GetLibraryResourceTypeFromExtension(const char* extension)
{
	ResourceType ret = ResourceType::RT_NULL;

	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		if ((*it)->CanLoadExtensionLibrary(extension))
		{
			ret = (*it)->GetLoaderType();
			break;
		}
	}

	return ret;
}

void ModuleResourceManager::LoadNewFile(std::string path)
{
	std::filesystem::path fsPath(path);

	if (!fsPath.has_extension())
	{
		return;
	}

	ResourceType type = GetAssetsResourceTypeFromExtension(fsPath.extension().string().c_str());

	if (type == RT_NULL)
	{
		return;
	}

	std::filesystem::path assetpath = App->file_system->GetAssetsPath();
	std::filesystem::path newPath = assetpath / fsPath.filename();

	bool couldCopyPaste = App->file_system->FileCopyPaste(fsPath, newPath, false);

	if (!couldCopyPaste)
	{
		return;
	}

	uuids::uuid uid = applic->resourceManager->NewGuid();

	CreateMetaForAsset(newPath, uid);

	ResourceLoader* loader = GetLoader(type);

	bool couldCreateLibrary = loader->CreateLibraryFromAsset(newPath, uid);

	if (!couldCreateLibrary)
	{
		return;
	}

	Resource* resource = loader->LoadResourceFromLibrary(uid);

	if (resource == nullptr)
	{
		return;
	}

	resourcesLoaded[type].push_back(resource);
}

void ModuleResourceManager::CreateMetaForAsset(std::filesystem::path assetpath, uuids::uuid uid)
{
	std::filesystem::path metapath = assetpath;
	metapath.replace_extension(assetpath.extension().string() + ".meta");

	JSON_Doc meta = applic->json_module->CreateJSON(metapath.string().c_str());

	meta.SetUid("uid", uid);

	meta.Save();
}

void ModuleResourceManager::LoadResourcesFromAssets()
{
	//Mirar sobre los metas, mirar si existe, ver sie sta cargado en el library y subrilo a la ram

}

Resource* ModuleResourceManager::Get(std::string unique_id)
{
	return nullptr;
}

bool ModuleResourceManager::DeleteResource(std::string unique_id)
{
	//bool ret = false;

	//for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	//{
	//	ResourceLoader* loader = (*it);

	//	if (loader->DeleteResource(unique_id))
	//	{
	//		ret = true;
	//		break;
	//	}
	//}

	//return ret;
	return false;
}

bool ModuleResourceManager::IsAssetMeta(const char* filepath, const char* metapath)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		if (App->file_system->FileExists(metapath))
		{
			std::string filepath_meta = std::string(filepath) + ".meta";

			if (filepath_meta == std::string(metapath))
			{
				ret = true;
			}
		}
	}

	return ret;
}

bool ModuleResourceManager::FileIsMeta(const char* filepath)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

		if (deco_file.file_extension == "meta")
			ret = true;
	}

	return ret;
}

void ModuleResourceManager::AddLoader(ResourceLoader* loader)
{
	loaders.push_back(loader);
}

