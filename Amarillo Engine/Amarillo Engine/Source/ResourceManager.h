#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__
#include "Application.h"
#include "Module.h"
#include "Globals.h"
#include "Random.h"
#include <map>
#include "Random.h"


class ModuleResourceManager : public Module
{

public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	virtual ~ModuleResourceManager();

	bool Awake();
	bool Start();
	bool CleanUp();
};
#endif //__RESOURCE_MANAGER_H__
