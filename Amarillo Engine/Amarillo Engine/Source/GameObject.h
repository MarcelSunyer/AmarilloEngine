#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "glmath.h"
#include "../External/MathGeoLib/include/Math/float3x3.h"
#include "Component.h"
#include <vector>

class GameObject
{

	GameObject(const char name, float3x3 transform, float3x3 scale, Quat* rot);
	~GameObject();

	void AddComponent(ComponentTypes* type);

	//All transform settings

	float3x3 GetLocalPosition();
	float3x3 GetWorldPosition();

	float3x3 GetLocalScale();
	float3x3 GetGlobalScale();

	Quat* GetLocalRotation();

	GameObject* mParent;
	std::vector<GameObject*> mChildren;
	std::vector<GameObject*> mComponents;
};
#endif