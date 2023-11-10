#include "Application.h"

#include "Module.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleConsole.h"
#include "ModuleMesh.h"
#include "ModuleTexture.h"
#include "ModuleScene.h"

extern Application* applic = nullptr;

Application::Application()
{
	applic = this;

	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);
	console = new ModuleConsole(this);
	mesh = new ModuleMesh(this);
	texture = new ModuleTexture(this);
	scene = new ModuleScene(this);

	// They will CleanUp() in reverse order

	// Main Modules

	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(mesh);
	AddModule(texture);
	AddModule(scene);

	// Renderer last!
	AddModule(renderer3D);
	AddModule(editor);
	AddModule(console);
}

Application::~Application()
{
	for (std::vector<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret; ++it)
	{
		(*it)->Init();
	}

	// After all Init calls we call Start() in all modules
	editor->AddLog("-------------- Application Start --------------");

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret; ++it)
	{
		(*it)->Start();
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		(*it)->PreUpdate(dt);
	}

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		(*it)->Update(dt);
	}

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		(*it)->PostUpdate(dt);
	}


	// Add a FPS to the vector
	editor->AddFPS(dt);

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (std::vector<Module*>::reverse_iterator it = list_modules.rbegin(); it != list_modules.rend() && ret; ++it)
	{
		ret = (*it)->CleanUp();
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}
