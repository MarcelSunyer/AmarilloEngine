#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Color.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleJSON.h"
#include "ModuleCamera3D.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{
	type = ComponentTypes::CAMERA;
	camera = applic->camera->CreateCamera();
	parent->transform->world_position = float3(0,4,-10);
}

void ComponentCamera::Enable()
{

}

void ComponentCamera::Update()
{
	owner->UpdateCamera(camera);
}

void ComponentCamera::Disable()
{

}

void ComponentCamera::OnEditor()
{
	if (ImGui::CollapsingHeader("Component Camera"))
	{
		float near_plane = camera->GetNearPlaneDistance();
		if (ImGui::DragFloat("Near plane", &near_plane, 0.2f, 0.01f, 1000))
		{
			camera->SetNearPlaneDistance(near_plane);
		}

		float far_plane = camera->GetFarPlaneDistance();
		if (ImGui::DragFloat("Far plane", &far_plane, 10, near_plane, 10000))
		{
			camera->SetFarPlaneDistance(far_plane);
		}

		float fov = camera->GetVerticalFOV();
		if (ImGui::DragFloat("Field of view", &fov, 1, 1, 179.9f))
		{
			camera->SetFOV(fov);
		}
	}
}

void ComponentCamera::Serialize(JSON_Doc* json)
{
	json->SetString("Camera", "Camera");

}

void ComponentCamera::DebugDraw()
{
	float3 corners[8];
	camera->GetCorners(corners);
	applic->renderer3D->DebugDraw(camera->GetFrustum(), Color(0, 0, 255), true, 4);
}



