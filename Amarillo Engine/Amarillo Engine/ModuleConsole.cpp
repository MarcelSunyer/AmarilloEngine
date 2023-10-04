#include "Globals.h"
#include "Application.h"
#include "ModuleConsole.h"
#include "ModuleInput.h"


ModuleConsole::ModuleConsole(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleConsole::~ModuleConsole()
{}

// -----------------------------------------------------------------
bool ModuleConsole::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleConsole::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleConsole::Update(float dt)
{
	return UPDATE_CONTINUE;
}
