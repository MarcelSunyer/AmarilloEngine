#include "ComponentTransform.h"
#include "GameObject.h"

#include "../External/ImGUI/imgui.h"

ComponentTransform::ComponentTransform(GameObject* parent) :
	Component(parent), position(float3(0.0f, 0.0f, 0.0f)), rotation(Quat::identity), scale(float3(0.0f, 0.0f, 0.0f))
{
	transform = float4x4::FromTRS(position, rotation, scale);
	type = ComponentTypes::TRANSFORM;
};

ComponentTransform::ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation) :
	Component(parent), scale(scale), rotation(rotation), position(position)
{
	transform = float4x4::FromTRS(position, rotation, scale);
	type = ComponentTypes::TRANSFORM;
};

void ComponentTransform::Enable() {
	if (!this->active) {
		this->active = true;
		//ON ENABLE CODE
	}
}

void ComponentTransform::Disable() {
	if (this->active) {
		this->active = false;
		//ON DISABLE CODE
	}
}

void ComponentTransform::Update() {

}

void ComponentTransform::UpdateMatrix() {
	transform = float4x4::FromTRS(position, rotation, scale);
}

void ComponentTransform::OnEditor() {
	if (ImGui::CollapsingHeader("Component Transform"))
	{
		ImGui::InputFloat3("Transform", (float*)&position);
		ImGui::InputFloat3("Scale", (float*)&scale);
		ImGui::InputFloat4("Rotation", (float*)&rotation);
	}
}
