#include "GameObject.h"
#include <vector>


GameObject::GameObject(std::string name) : mName(name), active(true), parent(nullptr)
{
	transform = (ComponentTransform*)AddComponent(ComponentTypes::TRANSFORM);
	//texture = (ComponentTexture*)AddComponent(ComponentTypes::TEXTURE);
	InitBoundingBoxes();

}

GameObject::~GameObject()
{
	transform = nullptr;
	
	if (deleteGameObject && parent != nullptr) {
		parent->DeleteChild(this);
	}

	for (size_t i = 0; i < components.size(); ++i)
	{
		Component* component = components[i];
		if (component != nullptr)
		{
			delete components[i];
			components[i] = nullptr;

		}
	}

	for (size_t i = 0; i < children.size(); ++i)
	{
		delete children[i];
		children[i] = nullptr;
	}

}

bool GameObject::Enable() {
	if (!active) {
		active = true;

		// Realizar acciones específicas cuando se activa el objeto
		// Por ejemplo, habilitar el renderizado de la malla
		if (mesh != nullptr) {
			//mesh->SetVisibility(true);
		}

		// ... (otras acciones específicas que puedas necesitar)

		glEnable(GL_DEPTH_TEST); // Puedes habilitar la prueba de profundidad aquí o en otro lugar según tu lógica.

		return true;
	}
	return false;
}


bool GameObject::Disable()
{
	if (active) {
		active = false;

		// Realizar acciones específicas cuando se desactiva el objeto
		// Por ejemplo, deshabilitar el renderizado de la malla
		if (mesh != nullptr) {
			//mesh->SetVisibility(false);
		}

		// ... (otras acciones específicas que puedas necesitar)

		glDisable(GL_DEPTH_TEST); // Puedes deshabilitar la prueba de profundidad aquí o en otro lugar según tu lógica.

		return true;
	}
	return false;
}

void GameObject::Update()
{
	for (std::vector<Component*>::iterator co = components.begin(); co != components.end(); co++)
	{
		Component* component_update = *co;
		component_update->Update();
		

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
		UpdateBoundingBoxes();
		RenderBoundingBoxes();
	}
}

GameObject* GameObject::GetParent()
{
	return parent;
}

void GameObject::InitBoundingBoxes()
{
	obb.SetNegativeInfinity();
	globalAABB.SetNegativeInfinity();

	if(this->GetComponent(ComponentTypes::MESH) != NULL)
	{
		std::vector<float3> floatArray;
		floatArray.reserve(mesh->mesh->ourVertex.size());
		
		for (const auto& vertex : mesh->mesh->ourVertex) {
			floatArray.push_back(vertex.Position);
		}

		aabb.SetFrom(&floatArray[0], floatArray.size());
	}
}

void GameObject::UpdateBoundingBoxes()
{
	if (this != nullptr && this->transform != nullptr)
	{
		obb = aabb;
		obb.Transform(this->transform->transform);

		globalAABB.SetNegativeInfinity();
		globalAABB.Enclose(obb);
		
	}
	else
	{
		LOG("Error: GameObject or its transform is null");
	}
}

void GameObject::RenderBoundingBoxes()
{
	float3 verticesO[8];
	obb.GetCornerPoints(verticesO);
	applic->renderer3D->DrawBoundingBox(verticesO, float3(0, 255, 0));

	float3 verticesA[8];
	globalAABB.GetCornerPoints(verticesA);
	applic->renderer3D->DrawBoundingBox(verticesA, float3(0, 255, 0));
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
