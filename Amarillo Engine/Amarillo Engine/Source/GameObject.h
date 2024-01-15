#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "glmath.h"
#include <vector>
#include "Application.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentCamera.h"
#include "ModuleScene.h"
#include "../External/MathGeoLib/include/Math/float3.h"
#include "ResourceManager.h"
#include "ComponentScript.h"


class Component;
class GameObject;

class __declspec(dllexport) GameObject
{
	friend class Component;
	friend class ComponentCamera;
	friend class ComponentTransform;
	friend class ComponentScript;
	friend class ComponentMesh;
	friend class ResourceMesh;
	friend class ResourceTexture;
	friend class ResourcePrefab;
	friend class Resource;

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

	void DebugDraw();

	Component* AddComponent(ComponentTypes component);
	void AddComponent(Component* component);
	Component* GetComponent(ComponentTypes type);
	GameObject* AddChildren(GameObject* children);
	GameObject* GetParent();

	static void FreeArrayMemory(void*** array_);
	static uint FindGameObjectsWithTag(const char* tag_to_find, GameObject*** objects);
	void FindTags(const char* tag_to_find, std::vector<GameObject*>* objects);

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
	static GameObject* GetGameObjectFromUID(const std::vector<GameObject*>& gameObjects, const std::string& UID);

	void* GetComponentScript(const char* script_class_name);
	const void* GetComponentScript(const char* script_class_name) const;

	Component* GetComponentInChildren(const ComponentTypes type, bool recursive);
	const Component* GetComponentInChildren(const ComponentTypes type, bool recursive) const;

	uint GetComponentsInChildren(const ComponentTypes type, Component*** comp_array, bool recursive);

	const uint GetComponentsInChildren(const ComponentTypes type, Component*** comp_array, bool recursive) const;

	uint GetComponentsInParent(const ComponentTypes type, Component*** comp_array);
	const uint GetComponentsInParent(const ComponentTypes type, Component*** comp_array) const;

	uint GetComponentsScript(const char* script_class_name, void*** comp_array);

	const uint GetComponentsScript(const char* script_class_name, void*** comp_array) const;

	uint GetComponentsScriptInChildren(const char* script_class_name, void*** comp_array, bool recursive);

	const uint GetComponentsScriptInChildren(const char* script_class_name, void*** comp_array, bool recursive) const;

	uint GetComponentsScriptInParent(const char* script_class_name, void*** comp_array);

	const uint GetComponentsScriptInParent(const char* script_class_name, void*** comp_array) const;

	void* GetComponentScriptInParent(const char* script_class_name);

	const void* GetComponentScriptInParent(const char* script_class_name) const;

	Component* GetComponentInParent(const ComponentTypes type);

	const Component* GetComponentInParent(const ComponentTypes type) const;
	
	void GetComponentsChildren(const ComponentTypes type, std::vector<Component*>* to_fill, bool recursive);
	void GetComponentsChildren(const ComponentTypes type, std::vector<Component*>* to_fill, bool recursive) const;
	void GetComponentsScriptChildren(const char* script_calss_name, std::vector<void*>* to_fill, bool recursive);
	void GetComponentsScriptChildren(const char* script_calss_name, std::vector<void*>* to_fill, bool recursive) const;

	uint GetComponents(const ComponentTypes type, Component*** comp_array);

	// BASE
	template<class Comp>
	inline Comp* GetComponent();

	template<class Comp>
	inline std::vector<Comp*> GetComponents();


	// CONST
	template<class Comp>
	inline const Comp* GetComponent() const;

	template<class Comp>
	inline const std::vector<Comp*> GetComponents() const;


	const char* GetTag() const;

	void SearchToDelete();

	bool HasChildren() const;

	void RemoveComponent(Component* component);
	
	GameObject* GameObject::GetGameObjectByID(const std::string& id);


	//No use, only reference
	ComponentTransform* transform = nullptr;

public:

	ComponentTexture* texture = nullptr;
	ComponentMesh* mesh = nullptr;

public: 

	AABB aabb;
	OBB obb;



};


template<class Comp>
inline Comp* GameObject::GetComponent()
{
	for (uint i = 0; i < components.size(); ++i) {
		Comp* component = dynamic_cast<Comp*>(components[i]);
		if (component != nullptr) {
			return component;
		}
	}
	return nullptr;
}

template<class Comp>
inline std::vector<Comp*> GameObject::GetComponents()
{
	std::vector<Comp*> comps;
	for (uint i = 0; i < components.size(); ++i) {
		Comp* component = dynamic_cast<Comp*>(components[i]);
		if (component != nullptr) {
			comps.push_back(component);
		}
	}
	return comps;
}

template<class Comp>
inline const Comp* GameObject::GetComponent() const
{
	for (uint i = 0; i < components.size(); ++i) {
		Comp* component = dynamic_cast<Comp*>(components[i]);
		if (component != nullptr) {
			return component;
		}
	}
	return nullptr;
}

template<class Comp>
inline const std::vector<Comp*> GameObject::GetComponents() const
{
	std::vector<Comp*> comps;
	for (uint i = 0; i < components.size(); ++i) {
		Comp* component = dynamic_cast<Comp*>(components[i]);
		if (component != nullptr) {
			comps.push_back(component);
		}
	}
	return comps;
}

#endif