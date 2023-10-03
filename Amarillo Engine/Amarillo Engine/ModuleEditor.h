#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR



#include "Module.h"
#include "Globals.h"
#include <vector>


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
	
	std::vector<float> vecFPSLog;
	const char* lastValue = "";

public:  //Windows Settings
	float brightnessFactor = 1.0f;
	bool showWindow = true;
	float windowSizeX = 500;
	float windowSizeY = 200;// Default window size

public:
	bool IoConfirmer = false;

};


#endif // MODULE_EDITOR

