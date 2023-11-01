#pragma once
#include "Component.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"
#include "glmath.h"


class ComponentTransform : public Component {

private:
	float3 position;
	Quat rotation;
	float3 scale;
	float4x4 transform;

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	void inline SetPosition(float3 position);
	void inline SetRotation(Quat rotation);
	void inline SetScale(float3 scale);

	float3 inline GetPosition() const { return this->position; };
	Quat inline GetRotation() const { return this->rotation; };
	float3 inline GetScale() const { return this->scale; };
	float4x4 inline GetTransformMatrix() const { return this->transform; };

	ComponentTransform(GameObject* parent); //DEFAULTS TO 0,0
	ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation);
public:

	void UpdateMatrix(); //Call after every interaction with the matrix;  

	float3 new_position{ 31.0f, 232.0f, 3.0f };
	Quat new_rotation = Quat::FromEulerXYZ(0.0f, 0.0f, 90.0f);  // Rotar 90 grados en el eje Z
	float3 new_scale{213.0f, 2321.0f, 2.0f};
};