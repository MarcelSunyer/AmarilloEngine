#pragma once

#include "Globals.h"
#include "Timer.h"


#include<vector>

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleEditor;
class ModuleConsole;
class ModuleMesh;
class ModuleTexture;
class ModuleScene;




class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleConsole* console;
	ModuleMesh* mesh;
	ModuleTexture* texture;
	ModuleScene* scene;

private:

	Timer	ms_timer;
	float	dt;
	std::vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

};

extern Application* applic;