#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "../External/MathGeoLib/include/Math/float3.h"
#include "../External/MathGeoLib/include/Math/float4x4.h"
class GameObject;

enum ComponentTypes
{
	GEOMETRY,
	MATERIAL,
};

class Component
{

public:
	Component();
	~Component();

	//virtual void OnEditor();

	void SetActive(bool active);

	//GameObject* mOwner;

	ComponentTypes GetType();

public:
	bool active = false;

};

class C_Transform : public Component
{


	//void OnEditor() override;

	float3 mPosition;
	float3 mScale;
	Quat* mRotation;

	float4x4 mGlobalMatrix;
	float4x4 mLocalMatrix;

};


#endif