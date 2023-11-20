#include "ComponentCamera.h"
#include "Application.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{
	type = ComponentTypes::CAMERA;
	camera = applic->camera->CreateCamera();
}

void ComponentCamera::Enable()
{

}

void ComponentCamera::Update()
{

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
