#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "../External/MathGeoLib/include/Math/float3x3.h"
#include "glmath.h"
#include "Component.h"
#include "../External/MathGeoLib/include/Math/Quat.h"
#include <vector>

class GameObject
{
	GameObject(const char name, float3 transform, float3 scale, Quat* rot);
	~GameObject();

	void AddComponent(ComponentTypes types);

	//All transform settings

	float3 GetLocalPosition();
	float3 GetWorldPosition();

	float3 GetLocalScale();

	Quat* GetLocalRotation();

	GameObject* mParent;
	std::vector<GameObject*> mChildren;
	std::vector<Component*> mComponents;

	std::string mName;

	float3 translation = float3::zero;
	float3 scale = float3::one;
};
#endif