#pragma once
#include "Component.h"
#include "../External/MathGeoLib/include/Math/float3.h"
#include "../External/MathGeoLib/include/Math/float4.h"
#include "../External/MathGeoLib/include/Math/float4x4.h"
#include "../External/MathGeoLib/include/Math/Quat.h"

class GameObject;

class ComponentTransform : public Component {

public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	void SetWorldPosition(float3 position);
	void SetWorldRotation(Quat rotation);
	void SetWorldScale(float3 scale);
	void SetWorldRotationEuler(float3 rotation);
	

	void SetLocalPosition(float3 position);
	void SetLocalRotation(Quat rotation);
	void SetLocalScale(float3 scale);
	void SetLocalRotationEuler(float3 rotation);


	float3 GetPosition() const { return this->world_position; };
	Quat GetRotation() const { return this->world_rotation; };
	float3 GetScale() const { return this->world_scale; };
	float4x4 GetTransformMatrix() const { return this->world_matrix; };

	float3 GetScale( );

	ComponentTransform(GameObject* parent);
	ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation);

	float3 GetNormalizeAxis(int i);
	float3 GetForward();
public:
	void RecalculateTransformHierarchy();
public:

	void UpdateLocalMatrix(); 

	float3 world_position{ 0.0f, 0.0f, 0.0f };
	Quat world_rotation = Quat::FromEulerXYZ(0.0f, 0.0f, 0.0f);  
	float3 world_rotation_euler{ 0.0f, 0.0f, 0.0f };
	float3 world_scale{1.0f, 1.0f, 1.0f};
	float4x4 world_matrix = float4x4::identity;

	float3 local_position{ 0.0f, 0.0f, 0.0f };
	Quat local_rotation = Quat::FromEulerXYZ(0.0f, 0.0f, 0.0f); 
	float3 local_rotation_euler{ 0.0f, 0.0f, 0.0f };
	float3 local_scale{ 1.0f, 1.0f, 1.0f };
	float4x4 local_matrix = float4x4::identity;
	
};