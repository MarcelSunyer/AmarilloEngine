#include "ModuleInit.h"
#include "ModuleCamera3D.h"
#include "Primitive.h"


ModuleInit::ModuleInit(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	root_object = new GameObject("Scene");
	game_objects.push_back(root_object);
	selected_object = root_object;
}

ModuleInit::~ModuleInit()
{}

// Load assets
bool ModuleInit::Start()
{
	root_object = new GameObject("Juan");
	game_objects.push_back(root_object);
	selected_object = root_object;
	

	return true;
}

// Load assets
bool ModuleInit::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleInit::Update(float dt)
{
	for (uint n = 0; n < game_objects.size(); n++)
	{
		game_objects[n]->Update();
	}

	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleInit::PostUpdate(float dt)
{

	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleInit::AddGameObject(GameObject* object)
{
	selected_object = object;
	object->SetParent(root_object);
	root_object->AddChildren(object);
	game_objects.push_back(object);
	
}

