#pragma once
#include "Module.h"
#include "Globals.h"
#include <string>
#include <vector>

class ModuleConsole : public Module
{
public:
	ModuleConsole(Application* app, bool start_enabled = true);
	~ModuleConsole();

	bool Start();
	void StartConsole();
	void UpdateConsole();
	bool CleanUp();
public:
	std::vector<std::string> logs;
	
}; 
