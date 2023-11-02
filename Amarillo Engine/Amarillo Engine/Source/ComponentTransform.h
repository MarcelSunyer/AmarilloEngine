#pragma once
#include "Component.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"
#include "glmath.h"


class ComponentTransform : public Component {

public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	void inline SetPosition(float3 position);
	void inline SetRotation(Quat rotation);
	void inline SetScale(float3 scale);

	float3 inline GetPosition() const { return this->new_position; };
	Quat inline GetRotation() const { return this->new_rotation; };
	float3 inline GetScale() const { return this->new_scale; };
	float4x4 inline GetTransformMatrix() const { return this->transform; };

	ComponentTransform(GameObject* parent);
	ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation);
public:

	void UpdateMatrix(); 

	float3 new_position{ 0.0f, 0.0f, 0.0f };
	Quat new_rotation = Quat::FromEulerXYZ(0.0f, 0.0f, 0.0f);  
	float3 new_scale{1.0f, 1.0f, 1.0f};
	float4x4 transform;
};