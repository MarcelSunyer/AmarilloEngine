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

	//Per a poder afegir fills al game objects ho fas amb el child, pero dona problemes per al render de la mesh i l atextura, dirli al profe pq em cago al damunt
	child = new GameObject("Camera");
	child->AddComponent(ComponentTypes::CAMERA);
	root_object->AddChildren(child);


	return true;
}

update_status ModuleScene::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_W)) gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	else if (App->input->GetKey(SDL_SCANCODE_E)) gizmoOperation = ImGuizmo::OPERATION::ROTATE;
	else if (App->input->GetKey(SDL_SCANCODE_R)) gizmoOperation = ImGuizmo::OPERATION::SCALE;

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

	//ViewMatrixOpenGL()
	float4x4 viewMatrix = App->camera->frustum.ViewMatrix();
	viewMatrix = viewMatrix.Transposed();

	//ProjectionMatrixOpenGL()
	float4x4 projectionMatrix = App->camera->frustum.ProjectionMatrix();
	projectionMatrix = projectionMatrix.Transposed();

	float3 mPosition = App->editor->GameObject_selected->transform->GetPosition();
	float4x4 modelProjection = float4x4::Translate(mPosition);

	
	ImGuizmo::SetRect(0.0f, 0.0f, App->editor->w, App->editor->h);


	//TODO: Que no salga de la ventana "Scene" -> Ahora es "Debug" 
	///
	//ImVec2 windowPosition = ImGui::GetWindowPos();
	//ImVec2 contentRegionMax = ImGui::GetContentRegionMax();

	//int offset = ImGui::GetFrameHeight() / 2;
	//ImGuizmo::SetRect(windowPosition.x, windowPosition.y + offset, contentRegionMax.x, contentRegionMax.y);

	//ImGuizmo::SetDrawlist();
	///


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
