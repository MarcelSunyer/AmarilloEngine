#include "ResourceManager.h"
#include "Application.h"
#include "../External/ImGui/imgui.h"
#include "../External/Parson/parson.h"
#include "ModuleScene.h"
//Importers aqui


ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Awake()
{
	//Meter todos los loaders
	bool ret = true;
	
	return ret;
}

bool ModuleResourceManager::Start()
{
	return false;
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
	LoadFileToEngine(filepath);
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

ResourceType ModuleResourceManager::AssetExtensionToType(const char* extension)
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

ResourceType ModuleResourceManager::LibraryExtensionToType(const char* extension)
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

Resource* ModuleResourceManager::Get(std::string unique_id)
{
	return Get(unique_id, ResourceType::RT_NULL);
}

Resource* ModuleResourceManager::Get(std::string unique_id, ResourceType type)
{
	Resource* ret = nullptr;

	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		ResourceLoader* curr_loader = (*it);

		if (type == ResourceType::RT_NULL || type == curr_loader->GetLoaderType())
		{
			ret = curr_loader->GetResource(unique_id);

			if (ret != nullptr)
				break;
		}
	}
	return ret;
}
Resource* ModuleResourceManager::CreateNewResource(ResourceType type)
{
	std::string new_id = GetNewUID();
	return CreateNewResource(type, new_id);
}
Resource* ModuleResourceManager::CreateNewResource(ResourceType type, std::string unique_id)
{
	Resource* res = nullptr;

	res = Get(unique_id);

	if (res == nullptr)
	{
		ResourceLoader* loader = GetLoader(type);

		if (loader != nullptr)
		{
			res = loader->CreateResource(unique_id);

			loader->AddResource(res);
		}
	}

	return res;
}

bool ModuleResourceManager::DeleteResource(std::string unique_id)
{
	bool ret = false;

	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		ResourceLoader* loader = (*it);

		if (loader->DeleteResource(unique_id))
		{
			ret = true;
			break;
		}
	}

	return ret;
}
void ModuleResourceManager::LoadFileToEngine(const char* filepath, std::vector<Resource*>& resources)
{
	if (App->file_system->FileExists(filepath))
	{
		DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

		ResourceType type = AssetExtensionToType(deco_file.file_extension.c_str());

		if (type != ResourceType::RT_NULL)
		{
			bool can_load = false;

			std::string new_path;
			if (App->file_system->FileCopyPaste(filepath, App->file_system->GetAssetsPath().c_str(), false, new_path))
			{
				can_load = true;
			}
			else if (deco_file.path == App->file_system->GetAssetsPath())
			{
				new_path = filepath;
				can_load = true;
			}

			if (can_load)
			{
				deco_file = App->file_system->DecomposeFilePath(new_path.c_str());

				ResourceLoader* loader = GetLoader(type);

				if (loader != nullptr)
				{
					bool ret = loader->LoadFileToEngine(deco_file, resources);

					if (ret)
					{
						// SUCCES
					}
					else
					{
						// ERROR
					}
				}
			}
		}
	}
}

void ModuleResourceManager::UnloadAssetFromEngine(const char* filepath)
{
	if (App->file_system->FileExists(filepath))
	{
		DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(filepath);

		ResourceType type = AssetExtensionToType(d_filepath.file_extension.c_str());
		ResourceLoader* loader = GetLoader(type);

		if (loader != nullptr)
		{
			loader->RemoveAssetInfoFromEngine(d_filepath);
		}

		App->file_system->FileDelete(filepath);
	}
}

void ModuleResourceManager::RemoveAssetInfoFromEngine(const char* filepath)
{
	DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(d_filepath.file_extension.c_str());
	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		loader->RemoveAssetInfoFromEngine(d_filepath);
	}
}

void ModuleResourceManager::ClearResourceFromGameObjects(Resource* res)
{
	if (res != nullptr)
	{
		ResourceLoader* load = GetLoader(res->GetType());

		if (load != nullptr)
		{
			std::vector<GameObject*> game_object = App->scene->game_objects;

			for (std::vector<GameObject*>::iterator it = game_object.begin(); it != game_object.end(); ++it)
			{
				load->ClearFromGameObject(res, (*it));
			}
		}
	}
}

void ModuleResourceManager::ExportResourceToLibrary(Resource* resource)
{
	if (resource != nullptr)
	{
		ResourceLoader* loader = GetLoader(resource->GetType());

		if (loader != nullptr)
		{
			bool ret = loader->ExportResourceToLibrary(resource);

			if (ret)
			{
				// SUCCES
			}
			else
			{
				// ERROR
			}
		}
	}
}

void ModuleResourceManager::ImportResourceFromLibrary(const char* filepath)
{
	DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = LibraryExtensionToType(deco_file.file_extension.c_str());

	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		bool ret = loader->ImportResourceFromLibrary(deco_file);

		if (ret)
		{
			// SUCCES
		}
		else
		{
			// ERROR
		}
	}
}

void ModuleResourceManager::LoadAssetIntoScene(const char* filepath)
{
	DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(deco_file.file_extension.c_str());

	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		bool ret = loader->LoadAssetIntoScene(deco_file);

		if (ret)
		{
			// SUCCES
		}
		else
		{
			// ERROR
		}
	}
}

bool ModuleResourceManager::IsAssetOnLibrary(const char* filepath, std::vector<std::string>& library_files_used)
{
	bool ret = false;

	DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(deco_file.file_extension.c_str());

	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		ret = loader->IsAssetOnLibrary(deco_file, library_files_used);
	}

	return ret;
}

void ModuleResourceManager::ReimportAssetToEngine(const char* filepath)
{
	RemoveAssetInfoFromEngine(filepath);
	LoadFileToEngine(filepath);
}

void ModuleResourceManager::RenameAsset(const char* filepath, const char* new_name)
{
	DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(d_filepath.file_extension.c_str());

	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		bool ret = loader->RenameAsset(d_filepath, new_name);

		if (ret)
		{
			// SUCCES
		}
		else
		{
			// ERROR
		}
	}
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

std::string ModuleResourceManager::GetNewUID()
{
	return GetUIDRandomHexadecimal();
}

bool ModuleResourceManager::DrawResourceSelector(const char* id, const char* name, ResourceType type, Resource*& res)
{
	bool ret = false;

	std::string res_name = "empty";

	if (res != nullptr)
		res_name = res->GetFileName();

	std::string line_text = std::string(name) + ": ";

	ImGui::PushID(id);

	ImGui::Text(line_text.c_str());
	ImGui::SameLine();

	ImGui::TextColored({ 0.45f, 0.86f, 1, 1.0f }, res_name.c_str());
	ImGui::SameLine();

	if (ImGui::SmallButton("+"))
	{
		ImGui::OpenPopup("HerarchyPopup");
	}

	if (ImGui::BeginPopupContextItem("HerarchyPopup"))
	{
		ResourceLoader* loader = GetLoader(type);

		std::map<std::string, Resource*> resources = loader->GetResources();

		if (ImGui::Selectable("Null"))
		{
			res = nullptr;
			draw_resource_selector_win = false;
			ret = true;
		}

		ImGui::Separator();

		for (std::map<std::string, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
		{
			if ((*it).second != res)
			{
				ImGui::PushID((*it).second->GetUniqueId().c_str());

				if (ImGui::Selectable((*it).second->GetFileName().c_str()))
				{
					res = (*it).second;
					draw_resource_selector_win = false;
					ret = true;
				}

				ImGui::PopID();
			}

		}

		ImGui::EndPopup();
	}


	ImGui::PopID();

	return ret;
}

void ModuleResourceManager::AddLoader(ResourceLoader* loader)
{
	loaders.push_back(loader);
}

