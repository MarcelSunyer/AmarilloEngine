#include "ComponentTransform.h"
#include "GameObject.h"

#include "../External/ImGUI/imgui.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
{
	type = ComponentTypes::TRANSFORM;
	UpdateMatrix();

};

ComponentTransform::ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation) :
	Component(parent), new_scale(scale), new_rotation(rotation), new_position(position)
{

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

inline void ComponentTransform::SetPosition(float3 position)
{
	this->new_position = position;
	UpdateMatrix();
}

inline void ComponentTransform::SetRotation(Quat rotation)
{
	this->new_rotation = rotation;
	UpdateMatrix();
}

inline void ComponentTransform::SetScale(float3 scale)
{
	this->new_scale = scale;
	UpdateMatrix();
}
void ComponentTransform::UpdateMatrix() {

	transform = float4x4::FromTRS(new_position, new_rotation, new_scale);
}

void ComponentTransform::OnEditor() {
	
	if (ImGui::CollapsingHeader("Component Transform"))
	{
		float3 euler_rotation = new_rotation.ToEulerXYZ();
		float3 euler_degree = euler_rotation * RADTODEG;
		ImGui::DragFloat3("Position:", (float*)&new_position);
		ImGui::DragFloat3("Scale:", (float*)&new_scale);
		bool rot_change = ImGui::DragFloat3("Rotation:", (float*)&euler_degree);
		
		if (rot_change)
		{
			euler_rotation = euler_degree * DEGTORAD;
			new_rotation = Quat::FromEulerXYZ(euler_rotation.x, euler_rotation.y, euler_rotation.z);
		}
		
		UpdateMatrix();
	}
}


