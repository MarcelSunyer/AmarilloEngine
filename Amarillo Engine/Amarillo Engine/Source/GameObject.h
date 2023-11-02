#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "glmath.h"
#include <vector>
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"

class GameObject
{
public:
	GameObject(std::string name);
	GameObject(const char name, float3 transform, float3 scale, Quat* rot);
	~GameObject();
	
	bool Enable();
	bool Disable();

	void Update();
	
	void SetParent(GameObject* parent);
	
	Component* AddComponent(ComponentTypes component);
	Component* GetComponent(ComponentTypes type);
	std::vector<Component*> CreateComponent(ComponentTypes type);
	GameObject* AddChildren(GameObject* children);

public:
	std::string mName;

	GameObject* parent;
	std::vector<GameObject*> children;

	bool selected = false;
	bool active;

	std::vector<Component*> components;
	
	ComponentTransform* transform = nullptr;


};
#endif