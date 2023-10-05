#include "Globals.h"
#include "Application.h"
#include "ModuleConsole.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"
#include <string>
#include "ImGui/imgui.h"
#include "..\Amarillo Engine\SDL\include\SDL.h"
#include <iostream>



ModuleConsole::ModuleConsole(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleConsole::~ModuleConsole()
{}


bool ModuleConsole::Start()
{
	LOG("Setting the Console");
	bool ret = true;

	return ret;
}

void ModuleConsole::StartConsole()
{
	if (ImGui::Begin("Config"))
	{
		UpdateConsole();
	}
	ImGui::End();

}

void ModuleConsole::UpdateConsole()
{
	std::vector<std::string> logs;
	logs.push_back("This is a log message.");
	for (const std::string& log : logs) {
		ImGui::Text("%s", log.c_str());
	}
}

bool ModuleConsole::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}


