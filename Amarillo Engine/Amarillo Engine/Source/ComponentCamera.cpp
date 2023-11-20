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

		

	}

}
