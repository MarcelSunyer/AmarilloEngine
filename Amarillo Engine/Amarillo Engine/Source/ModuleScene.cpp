#include "ModuleScene.h"
#include "../External/ImGuizmo/ImGuizmo.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	app = app;
	ImGuizmo::Enable(true);
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	root_object = new GameObject("Scene");
	game_objects.push_back(root_object);

	GameObject* camera = CreateGameObject("Camera");
	camera->AddComponent(ComponentTypes::CAMERA);

	return true;
}

update_status ModuleScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_W) && !ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
		gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	else if (App->input->GetKey(SDL_SCANCODE_E) && !ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
		gizmoOperation = ImGuizmo::OPERATION::ROTATE;
	else if (App->input->GetKey(SDL_SCANCODE_R) && !ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
		gizmoOperation = ImGuizmo::OPERATION::SCALE;

	UpdateGameObjects();

	return update_status();
}

bool ModuleScene::CleanUp()
{

	return false;
}

GameObject* ModuleScene::CreateGameObject(std::string name)
{
	GameObject* newObject = new GameObject(name);
	game_objects.push_back(newObject);
	newObject->SetParent(root_object);
	return newObject;
}

GameObject* ModuleScene::LoadMeshAndTexture(std::string path_mesh, std::string path_tex)
{
	GameObject* gameObject = App->mesh->LoadMesh(path_mesh.c_str());
	App->texture->LoadTextureToGameObject(gameObject, path_tex);

	return gameObject;
}

GameObject* ModuleScene::CreateChildObject(std::string name)
{

	GameObject* newObject = new GameObject(name);
	root_object->AddChildren(newObject);
	newObject->SetParent(root_object);
	return newObject;
}

std::vector<GameObject*> ModuleScene::GetGameObjects()
{
	return game_objects;
}

void ModuleScene::ImGuizmoHandling()
{

	ImGuizmo::BeginFrame();
	if (App->editor->GameObject_selected == nullptr) return;

	ComponentTransform* selected_transform = (ComponentTransform*)App->editor->GameObject_selected->GetComponent(ComponentTypes::TRANSFORM);

	//TODO: Probar a cambiar las Transposed() por no transposed y mirar que el SetRect() tenga el tamaño de la textura de la ventana "Scene"

	//ViewMatrix from OpenGL
	float4x4 viewMatrix = App->camera->editor_camera->Camera_frustum.ViewMatrix();
	viewMatrix = viewMatrix.Transposed();

	//ProjectionMatrix from OpenGL
	float4x4 projectionMatrix = App->camera->editor_camera->Camera_frustum.ProjectionMatrix();
	projectionMatrix = projectionMatrix.Transposed();

	float3 mPosition = App->editor->GameObject_selected->transform->GetPosition();
	float4x4 modelProjection = float4x4::Translate(mPosition);

	
	ImGuizmo::SetRect(App->editor->windowPosition.x, App->editor->windowPosition.y + App->editor->offset, App->editor->size_texture_scene.x, App->editor->size_texture_scene.y);
	

	//TODO: Que no salga de la ventana "Scene" -> Ahora es "Debug" 
	
/*	ImVec2 windowPosition = ImGui::GetWindowPos();
	ImVec2 contentRegionMax = ImGui::GetContentRegionMax();

	int offset = ImGui::GetFrameHeight() / 2;
	ImGuizmo::SetRect(windowPosition.x, windowPosition.y + offset, contentRegionMax.x, contentRegionMax.y);

	ImGuizmo::SetDrawlist()*/;


	ComponentTransform* trans = App->editor->GameObject_selected->transform;
	float4x4 mat = trans->GetTransformMatrix().Transposed();

	//gizmoOperation
	float modelPtr[16];
	memcpy(modelPtr, modelProjection.ptr(), 16 * sizeof(float));

	ImGuizmo::MODE finalMode = (gizmoOperation == ImGuizmo::OPERATION::SCALE ? ImGuizmo::MODE::LOCAL : guizmoMode);
	
	ImGuizmo::Manipulate(viewMatrix.v[0], projectionMatrix.v[0], gizmoOperation, finalMode, mat.ptr());
	//ImGuizmo::Manipulate(viewMatrix.ptr(), projectionMatrix.ptr(), gizmoOperation, finalMode, modelPtr);


	if (ImGuizmo::IsUsing())
	{
		//Reformat ImGuizmo Transform output to our matrix
		/*float4x4 newMatrix;
		newMatrix.Set(modelPtr);
		modelProjection = newMatrix;*/


		//Set Global Transform 
		//selected_transform->SetGlobalTransform(modelProjection);
	}
}

void ModuleScene::UpdateGameObjects()
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		GameObject* update = *it;
		update->Update();
	}
}

void ModuleScene::DebugDrawGameObjects()
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		GameObject* update = *it;
		update->DebugDraw();
	}
}

