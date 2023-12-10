#include "GameObject.h"
#include <vector>


GameObject::GameObject()
{
	this->mName = "";
	parent = nullptr;

	active = true;
	selected = false;

	AddComponent(new ComponentTransform(this));
}

GameObject::GameObject(std::string name) : mName(name), active(true), parent(nullptr)
{
	transform = (ComponentTransform*)AddComponent(ComponentTypes::TRANSFORM);
}

GameObject::~GameObject()
{
	transform = nullptr;
}

void GameObject::Enable() 
{
	if (active) 
	{
		return;
	}
	active = true;
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		Component* component_enable = *it;
		component_enable->Enable();
	}
}


void GameObject::Disable()
{
	if (!active)
	{
		return;
	}
	active = false;

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		Component* component_enable = *it;
		component_enable->Disable();
	}
}

void GameObject::Update()
{
	for (std::vector<Component*>::iterator co = components.begin(); co != components.end(); co++)
	{
		Component* component_update = *co;
		component_update->Update();
	}

	for (auto it = applic->scene->game_objects.begin(); it != applic->scene->game_objects.end(); ++it) {

		if ((*it)->UID == this->UID && (*it) != this) { // If it is repeated, regenerate

			this->UID = applic->resourceManager->NewGuid().toString();

		}

	}
}

bool GameObject::SetParent(GameObject* newParent)
{
	if (parent != nullptr) 
	{
		if (newParent->IsChildOf(this))
		{
			return false;
		}
		parent->DeleteChild(this);
	}

	parent = newParent;
	newParent->children.push_back(this);

	return true;
}

void GameObject::RemoveParent()
{
	if (parent == nullptr)
	{
		return;
	}
	parent->DeleteChild(this);
}

bool GameObject::IsChildOf(GameObject* gameobject)
{
	if (gameobject == this)
	{
		return true;
	}
	if (gameobject->children.empty())
	{
		return false;
	}
	for (size_t i = 0; i < gameobject->children.size(); i++)
	{
		if (IsChildOf(gameobject->children[i]))
		{
			return true;
		}
	}
	return false;
}

void GameObject::DeleteChild(GameObject* child)
{
	for (int i = 0; i < children.size(); i++) {
		if (children[i] == child) {
			children.erase(children.begin() + i);
			child->parent = nullptr;
		}
	}
}

void GameObject::DebugDraw()
{
	for (std::vector<Component*>::iterator co = components.begin(); co != components.end(); co++)
	{
		Component* component_update = *co;
		component_update->DebugDraw();
	}
}

GameObject* GameObject::GetParent()
{
	return parent;
}

Component* GameObject::AddComponent(ComponentTypes type)
{
	Component* ret = nullptr;

	switch (type)
	{
	case(ComponentTypes::NONE):
		throw std::runtime_error("Component type not found");
		break;
	case(ComponentTypes::TRANSFORM):
		ret = new ComponentTransform(this);
		break;
	case(ComponentTypes::MESH):
		ret = new ComponentMesh(this);
		break;
	case(ComponentTypes::TEXTURE):
		ret = new ComponentTexture(this);
		break;
	case(ComponentTypes::CAMERA):
		ret = new ComponentCamera(this);
		break;
	default:
		throw std::runtime_error("Component type not found");
		break;
	}

	//components.push_back(ret);
	components.push_back(ret);
	return ret;
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

GameObject* GameObject::AddChildren(GameObject* children) 
{
	children->SetParent(this);

	return children;
}

Component* GameObject::GetComponent(ComponentTypes type)
{
	std::vector<Component*>::iterator item = components.begin();

	for (; item != components.end(); ++item) {

		if ((*item)->type == type) {
			return (*item);
		}
	}

	return nullptr;
}

void GameObject::UpdateCamera(Camera3D* camera)
{
	camera->SetPosition(transform->world_position);
}

GameObject* GameObject::GetGameObjectFromUID(const std::vector<GameObject*>& gameObjects, const std::string& UID)
{
	GameObject* gameObjectWithUID = nullptr;

	for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it) {

		if ((*it)->UID == UID) {

			gameObjectWithUID = (*it);

		}

	}

	return gameObjectWithUID;
}