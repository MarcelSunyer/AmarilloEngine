#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Globals.h"


class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	void DrawEditor();
	bool CleanUp() override;

	void AddFPS();

public:
	bool IoConfirmer = false;

};


#endif // MODULE_EDITOR

