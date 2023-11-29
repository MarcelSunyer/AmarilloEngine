#include "ComponentTransform.h"
#include "GameObject.h"

#include "../External/ImGUI/imgui.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
{
	type = ComponentTypes::TRANSFORM;
	UpdateLocalMatrix();

};

ComponentTransform::ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation) :
	Component(parent), world_scale(scale), world_rotation(rotation), world_position(position)
{

}

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

inline void ComponentTransform::SetWorldPosition(float3 position)
{
	float3 parentWorldPosition = float3::zero;
	if (owner->parent != nullptr)
	{
		parentWorldPosition = owner->parent->transform->world_position;
	}
	float3 newLocalPostion = position - parentWorldPosition;
	SetLocalPosition(newLocalPostion);
}

inline void ComponentTransform::SetWorldRotation(Quat rotation)
{
	Quat parentWorldRotation = Quat::identity;
	if (owner->parent != nullptr)
	{
		parentWorldRotation = owner->parent->transform->world_rotation;
	}
	Quat parentWorldRotationInverse = parentWorldRotation;
	parentWorldRotationInverse.Inverse();
	
	Quat newLocalRotation = rotation.Mul(parentWorldRotation);
	SetLocalRotation(newLocalRotation);

}

inline void ComponentTransform::SetWorldScale(float3 scale)
{
	float3 parentWorldScale = float3::one;
	if (owner->parent != nullptr)
	{
		parentWorldScale = owner->parent->transform->world_scale;
	}
	float3 newLocalScale = scale - parentWorldScale;
	SetLocalScale(newLocalScale);

}

inline void ComponentTransform::SetWorldRotationEuler(float3 rotation)
{
	Quat quaternion = Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z);

	SetWorldRotation(quaternion);
}

inline void ComponentTransform::SetLocalPosition(float3 position)
{
	this->local_position = position;
	UpdateLocalMatrix();
	RecalculateTransformHierarchy();
}

inline void ComponentTransform::SetLocalRotation(Quat rotation)
{
	this->local_rotation = rotation;
	this->local_rotation_euler = local_rotation.ToEulerXYZ();
	UpdateLocalMatrix();
	RecalculateTransformHierarchy();
}

inline void ComponentTransform::SetLocalScale(float3 scale)
{
	this->local_scale = scale;
	UpdateLocalMatrix();
	RecalculateTransformHierarchy();
}
inline void ComponentTransform::SetLocalRotationEuler(float3 rotation)
{
	local_rotation_euler = rotation;
	local_rotation = Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z);

	UpdateLocalMatrix();
	RecalculateTransformHierarchy();
}



float3 ComponentTransform::GetScale()
{
	return world_scale;
}

void ComponentTransform::UpdateLocalMatrix() {

	local_matrix = float4x4::FromTRS(local_position, local_rotation, local_scale);
}

void ComponentTransform::RecalculateTransformHierarchy()
{
	std::vector<ComponentTransform*> torecalculate;

	torecalculate.push_back(this);

	while (!torecalculate.empty())
	{
		ComponentTransform* recalculate = *torecalculate.begin();
		
		torecalculate.erase(torecalculate.begin());

		for (std::vector<GameObject*>::iterator dt = recalculate->owner->children.begin(); dt != recalculate->owner->children.end(); dt++)
		{
			torecalculate.push_back((*dt)->transform);
		}

		float4x4 parentWorldMatrix = float4x4::identity;

		if (recalculate->owner->parent != nullptr)
		{
			parentWorldMatrix = recalculate->owner->parent->transform->world_matrix;
		}	
		recalculate->world_matrix = parentWorldMatrix * recalculate->local_matrix;

		recalculate->world_matrix.Decompose(world_position, world_rotation, world_scale);
		this->world_rotation_euler = world_rotation.ToEulerXYZ();
	}
}

void ComponentTransform::OnEditor() {
	
	if (ImGui::CollapsingHeader("Component Transform"))
	{
		float3 newworldposition = local_position;

		float3 newsworldscale = local_scale;

		float3 new_euler_degree = local_rotation_euler * RADTODEG;

		bool pos_change = ImGui::DragFloat3("Position:", (float*)&newworldposition);
		bool scale_change = ImGui::DragFloat3("Scale:", (float*)&newsworldscale);
		bool rot_change = ImGui::DragFloat3("Rotation:", (float*)&new_euler_degree);
		
		if (pos_change)
		{
			SetLocalPosition(newworldposition);

		}
		if (scale_change)
		{
			SetLocalScale(newsworldscale);
		}
		
		if (rot_change)
		{
			new_euler_degree = new_euler_degree * DEGTORAD;

			SetLocalRotationEuler(new_euler_degree);
		}
	}
}


