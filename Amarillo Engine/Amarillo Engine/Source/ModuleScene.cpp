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

	GameObject* camera = CreateGameObject("Camera", nullptr);
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

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{

		//App->camera->active_camera->OnClick(App->input->GetMouseX(), App->window->GetWindowSize()[0] - App->input->GetMouseY());
	}

	UpdateGameObjects();

	//Actua
	
	return update_status();
}

bool ModuleScene::CleanUp()
{

	return false;
}

GameObject* ModuleScene::CreateGameObject(std::string name,GameObject* parent)
{
	GameObject* newObject = new GameObject(name);

	if (parent == nullptr)
	{
		newObject->SetParent(root_object);

	}
	else
	{
		parent->AddChildren(newObject);
	}
	game_objects.push_back(newObject);
	return newObject;
}

void ModuleScene::DeleteGameObject(GameObject* gameobject)
{
	std::vector<GameObject*> todelete;

	todelete.push_back(gameobject);
	gameobject->RemoveParent();

	while (!todelete.empty())
	{
		GameObject* deletes = *todelete.begin();

		todelete.erase(todelete.begin());

		deletes->Disable();

		for (std::vector<GameObject*>::iterator dt = deletes->children.begin(); dt != deletes->children.end(); dt++)
		{
			todelete.push_back(*dt);
		}
		deletes->children.clear();

		for (size_t i = 0; i < deletes->components.size(); ++i)
		{
			Component* component = deletes->components[i];
			delete deletes->components[i];
		}
		deletes->components.clear();

		if (deletes == App->editor->GameObject_selected)
		{
			App->editor->GameObject_selected = nullptr;
		}
	}
}

GameObject* ModuleScene::LoadMeshAndTexture(std::string path_mesh, std::string path_tex)
{
	GameObject* gameObject = App->mesh->LoadMesh(path_mesh.c_str());

	for (std::vector<GameObject*>::iterator it = gameObject->children.begin(); it != gameObject->children.end(); it++)
	{
		App->texture->LoadTextureToGameObject((*it), path_tex);
	}
	
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

	//ViewMatrix from OpenGL
	float4x4 viewMatrix = App->camera->editor_camera->Camera_frustum.ViewMatrix();
	viewMatrix = viewMatrix.Transposed();

	//ProjectionMatrix from OpenGL
	float4x4 projectionMatrix = App->camera->editor_camera->Camera_frustum.ProjectionMatrix();
	projectionMatrix = projectionMatrix.Transposed();

	float3 mPosition = App->editor->GameObject_selected->transform->GetPosition();
	float4x4 modelProjection = float4x4::Translate(mPosition);

	ImGuizmo::SetRect(App->editor->windowPosition.x, App->editor->windowPosition.y + App->editor->offset, App->editor->size_texture_scene.x, App->editor->size_texture_scene.y);

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
		float4x4 newMatrix;
		newMatrix.Set(modelPtr);
		modelProjection = newMatrix;

		//Column or Row? idk
		float3 firtsCol = float3(modelProjection[0][0], modelProjection[1][0], modelProjection[2][0]);

		//Set Global Transform 
		selected_transform->world_position = firtsCol;
	}
}

void ModuleScene::UpdateGameObjects()
{
	
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		GameObject* update = *it;
		if (!update->active)
		{
			continue;
		}
		update->Update();
	}
}

void ModuleScene::DebugDrawGameObjects()
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		GameObject* update = *it;
		if (!update->active)
		{
			continue;
		}
		update->DebugDraw();
	}
}

void ModuleScene::TestGameObjectSelection(const LineSegment& ray)
{
	std::map<float, GameObject*> game_object_candidates;
	App->editor->GameObject_selected = nullptr;

	for (uint i = 0; i < game_objects.size(); i++)
	{
		//We don't test against meshless components. 
		if (game_objects[i]->GetComponent(ComponentTypes::MESH) == nullptr)
		{
			continue;
		}

		if (ray.Intersects(game_objects[i]->aabb))
		{
			float closest, furthest;
			if (ray.Intersects(game_objects[i]->obb, closest, furthest)) //Intersect has issues with near-0 sizes, so sometimes misses the Plane :/
			{
				game_object_candidates[closest] = game_objects[i];
			}
		}
	}

	//I Initially wanted to do a <algorythm>Sort, but a custom operator couldn't take in external stuff like ray.

	std::vector<GameObject*> game_objects_sorted;

	//Turn map to vector cause iterating vectors is easy :)
	for (std::map<float, GameObject*>::iterator i = game_object_candidates.begin(); i != game_object_candidates.end(); i++)
	{
		game_objects_sorted.push_back(i->second);
	}

	for (uint i = 0; i < game_objects_sorted.size(); i++)
	{
		ComponentMesh* mesh_to_test = (ComponentMesh*)game_objects_sorted[i]->GetComponent(ComponentTypes::MESH);
		ModuleMesh::Mesh* mesh = mesh_to_test->mesh;
		if (mesh != nullptr)
		{
			LineSegment local_ray = ray;

			ComponentTransform* c_transform = (ComponentTransform*)game_objects_sorted[i]->GetComponent(ComponentTypes::TRANSFORM);

			local_ray.Transform(c_transform->GetTransformMatrix().Inverted());

			//Iterate points in a mesh jump 3 by 3 because triangles
			for (uint j = 0; j < mesh->indices.size(); j += 3) {
				uint triangle_indices[3] = { mesh->indices[j], mesh->indices[j + 1], mesh->indices[j + 2] };

				float3 point_a(mesh->ourVertex[triangle_indices[0]].Position.x, mesh->ourVertex[triangle_indices[0]].Position.y, mesh->ourVertex[triangle_indices[0]].Position.z);
				float3 point_b(mesh->ourVertex[triangle_indices[1]].Position.x, mesh->ourVertex[triangle_indices[1]].Position.y, mesh->ourVertex[triangle_indices[1]].Position.z);
				float3 point_c(mesh->ourVertex[triangle_indices[2]].Position.x, mesh->ourVertex[triangle_indices[2]].Position.y, mesh->ourVertex[triangle_indices[2]].Position.z);

				Triangle triangle(point_a, point_b, point_c);

				if (local_ray.Intersects(triangle, nullptr, nullptr)) {
					App->editor->GameObject_selected = game_objects_sorted[i];
					return;
				}
			}



		}
	}
}

