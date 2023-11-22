#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "..\External\SDL/include/SDL.h"
#include "../External/MathGeoLib/include/Math/float2.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	void InitLogs();

	float2 GetWindowSize();
public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	
};

#endif // __ModuleWindow_H__