#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "../External/MathGeoLib/include/Math/float3x3.h"
#include "glmath.h"
#include "../External/MathGeoLib/include/Math/Quat.h"
#include <vector>
#include "Component.h"

class GameObject
{
public:
	GameObject(const char name, float3 transform, float3 scale, Quat* rot);
	~GameObject();
	
	
	GameObject(std::string name);

	bool Enable(); //Start up + bool toggle
	bool Disable();//Clean up + bool toggle

	void Update();

	void SetParent(GameObject* parent);

	Component* AddComponent(Component* component);
	Component* GetComponent(ComponentTypes type);
	std::vector<Component*> GetComponents(ComponentTypes type);
	GameObject* AddChildren(GameObject* children);

public:
	std::string mName;

	std::vector<Component*> components;
	bool active;

	GameObject* parent;
	std::vector<GameObject*> children;
};
#endif