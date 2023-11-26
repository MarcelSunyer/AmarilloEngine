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
#include "../External/MathGeoLib/include/Geometry/LineSegment.h"
#include "../External/MathGeoLib/include/Geometry/Triangle.h"
#include "../External/MathGeoLib/include/MathGeoLibFwd.h"

#include <map>
#include <vector>


class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init() override;
	update_status Update(float dt);
	bool CleanUp();

	GameObject* CreateGameObject(std::string name, GameObject* parent);

	void DeleteGameObject(GameObject* gameobject);

	GameObject* LoadMeshAndTexture(std::string path_mesh, std::string path_tex);

	GameObject* CreateChildObject(std::string name);
	std::vector<GameObject*> GetGameObjects();

	void ImGuizmoHandling();

	void UpdateGameObjects();

	void DebugDrawGameObjects();

	void TestGameObjectSelection(const LineSegment& ray);

public:
	std::vector<GameObject*> game_objects; 
	GameObject* root_object = nullptr;


	//ImGuizmo
	ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE guizmoMode = ImGuizmo::MODE::WORLD;

};

#endif //MODULE_IMPORTER
