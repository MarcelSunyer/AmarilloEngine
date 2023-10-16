#pragma once

#ifndef MODULE_GAMEOBJECT
#define MODULE_GAMEOBJECT

#include "Module.h"
#include "Application.h"


class ModuleGameObject : public Module
{
public:
	ModuleGameObject(Application* app);
	~ModuleGameObject();
};

#endif