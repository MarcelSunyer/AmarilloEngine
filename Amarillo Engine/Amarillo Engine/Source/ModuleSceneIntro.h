#pragma once
#include "Module.h"
#include "Application.h"
#include "Globals.h"
#include <vector>
#include "GameObject.h"
#include "Primitive.h"

using namespace std;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void AddGameObject(GameObject* object);

private:
	std::vector<Primitive*> primitives;

public:
	std::vector<GameObject*> game_objects;

	GameObject* root_object = nullptr;
	GameObject* selected_object = nullptr;
};
