#ifndef MODULE_SCENE
#define MODULE_SCENE

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "ModuleTexture.h"
#include <vector>
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "../External/ImGuizmo/ImGuizmo.h"


class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init() override;
	update_status Update();
	bool CleanUp();

	GameObject* CreateGameObject(std::string name);

	GameObject* LoadMeshAndTexture(std::string path_mesh, std::string path_tex);

	GameObject* CreateChildObject(std::string name);
	std::vector<GameObject*> GetGameObjects();

	void ImGuizmoHandling();

public:
	std::vector<GameObject*> game_objects; 
	GameObject* root_object = nullptr;
	GameObject* child = nullptr;

	//ImGuizmo
	ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE guizmoMode = ImGuizmo::MODE::WORLD;

};

#endif //MODULE_IMPORTER
