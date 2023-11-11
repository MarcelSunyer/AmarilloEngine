#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "glmath.h"
#include <vector>
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentCamera.h"

class GameObject
{
public:
	GameObject(std::string name);
	~GameObject();
	
	bool Enable();
	bool Disable();

	void Update();
	
	void SetParent(GameObject* parent);
	
	bool SetNewParent(GameObject* newParent);
	bool IsChildOf(GameObject* parent);
	void DeleteChild(GameObject* child);


	Component* AddComponent(ComponentTypes component);
	Component* GetComponent(ComponentTypes type);
	GameObject* AddChildren(GameObject* children);
	GameObject* GetParent();

public:
	std::string mName;

	GameObject* parent;
	std::vector<GameObject*> children;

	bool selected = false;
	bool active;

	std::vector<Component*> components;

	//No use, only reference
	ComponentTransform* transform = nullptr;
private:
	ComponentTexture* texture = nullptr;


};
#endif