#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "GameObject.h"

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

	float3x3 mPosition;
	float3x3 mScale;
	Quat* mRotation;

	float4x4 mGlobalMatrix;
	float4x4 mLocalMatrix;

};


#endif