#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"

#include "ComponentMesh.h"
#include <vector>

GameObject::GameObject(std::string name) : mName(name), active(true)
{
	AddComponent(ComponentTypes::TRANSFORM);

}

bool GameObject::Enable() //Start up + bool toggle
{
	if (!active) {
		active = true;
		return true;
		//StartUp here...
	}
	return false;
}

bool GameObject::Disable() 
{
	if (active) {
		active = false;
		return true;
		//CleanUp here...
	}
	return false;
}

void GameObject::Update()
{
	
}

void GameObject::CreatGameObject()
{
	parent = new GameObject("");
	children.push_back(parent);
	
}

void GameObject::SetParent(GameObject* parent)
{
	this->parent = parent;
}

Component* GameObject::AddComponent(ComponentTypes type)
{
	Component* ret = nullptr;

	switch (type)
	{
	case(ComponentTypes::NONE):
		//LOG("Component Type Error! Something broke...");
		break;
	case(ComponentTypes::TRANSFORM):
		ret = new ComponentTransform(this);
		break;
	case(ComponentTypes::MESH):
		ret = new ComponentMesh(this);
		break;
	}

	//components.push_back(ret);
	components.push_back(ret);
	return ret;
}

GameObject* GameObject::AddChildren(GameObject* children) {

	if (this->children.empty())
	{
		this->children.push_back(children);
	}
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

std::vector<Component*> GameObject::CreateComponent(ComponentTypes type)
{
	std::vector<Component*>::iterator item = components.begin();

	std::vector<Component*> ret;

	for (; item != components.end(); ++item) {
		if ((*item)->type == type) {
			ret.push_back((*item));
		}
	}

	// Crea un nuevo componente de tipo MeshComponent
	if (type == ComponentTypes::MESH) {
		ComponentMesh* newMeshComponent = new ComponentMesh(parent); // No se pasa ningún argumento al constructor
		components.push_back(newMeshComponent);
		ret.push_back(newMeshComponent);
	}

	return ret;
}