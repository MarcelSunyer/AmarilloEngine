#pragma once
#include "Globals.h"
#include "Module.h"
#include <string>


struct Logs
{
	std::string logs; //Msg
	int rep;

};

class ModuleConsole : public Module
{
public:

	
	ModuleConsole(Application* app, bool start_enabled = true);
	~ModuleConsole();

	void UpdateConsole();
	void AddLog(std::string msg);

	Logs logs_array[500]; //Podriamos limitarla

protected:
	int logs_array_size = 0;
};
