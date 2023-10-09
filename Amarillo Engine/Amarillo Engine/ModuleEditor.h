#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Globals.h"
#include <vector>
#include <iostream>
#include <string> 



class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	void DrawEditor();
	bool CleanUp() override;

	void AddFPS(float FPS);

	void ShowAboutInfo();

	void AdjustBrightness(float factor);	

	bool StyleTypes(const char* label);
	
	std::vector<float> vecFPSLog;
	const char* lastValue = "";

public:  //Windows Settings
	//Brightness value
	float brightnessFactor = 1.5f;

	
	//Show config window
	bool showWindow = true;
	
public:
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;
	bool IoConfirmer = false;

protected:
	//Console 
	int logs_array_size = 0;
	//Meterle el psuh en el Log con el Mconsole


};

struct Logs
{
	std::string logs; //Msg
	int rep;

};


class Console : public ModuleEditor
{
public:

	Logs logs_array[]; //Podriamos limitarla
	void UpdateConsole();
	void AddLog(std::string msg);


};

#endif // MODULE_EDITOR

