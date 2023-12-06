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


