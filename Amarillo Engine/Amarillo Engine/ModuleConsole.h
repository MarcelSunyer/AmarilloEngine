#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleConsole : public Module
{
public:
	ModuleConsole(Application* app, bool start_enabled = true);
	~ModuleConsole();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
}; 
