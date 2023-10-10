#pragma once
#include "Globals.h"
#include <string>

struct Logs
{
	std::string logs; //Msg
	int rep;

};

class Console
{
public:

	Logs logs_array[100]; //Podriamos limitarla
	void UpdateConsole();
	void AddLog(std::string msg);

protected:
	int logs_array_size = 0;
};
