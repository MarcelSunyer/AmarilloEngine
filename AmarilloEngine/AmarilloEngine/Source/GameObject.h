#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include "Component.h"
#include "ComponentTexture.h"

#include "../External/MathGeoLib/include/Geometry/AABB.h"
#include "../External/MathGeoLib/include/Geometry/OBB.h"

class Camera3D;
class ComponentMesh;
class ComponentTransform;

class GameObject
{
public:
	GameObject();
	GameObject(std::string name);
	~GameObject();


	void Enable();
	void Disable();

	void Update();
	
	bool SetParent(GameObject* parent);
	void RemoveParent();
	bool IsChildOf(GameObject* parent);
	void DeleteChild(GameObject* child);

	void StartPlay();

	void DebugDraw();

	Component* AddComponent(ComponentTypes component);
	void AddComponent(Component* component);
	Component* GetComponent(ComponentTypes type);
	GameObject* AddChildren(GameObject* children);
	GameObject* GetParent();
	void DeleteGameObject(GameObject* obj);

public:
	std::string mName;
	std::string tag;
	std::string UID;

	GameObject* parent;
	std::vector<GameObject*> children;

	bool selected = false;
	bool active = true;
	bool deleteGameObject;

	//TODO: Use tags vector for something
	std::vector<std::string> tags;

	std::vector<Component*> components;

	void UpdateCamera(Camera3D* camera);

	//No use, only reference
	ComponentTransform* transform = nullptr;

public:

	ComponentTexture* texture = nullptr;
	ComponentMesh* mesh = nullptr;

public: 

	AABB aabb;
	OBB obb;

};

#endif