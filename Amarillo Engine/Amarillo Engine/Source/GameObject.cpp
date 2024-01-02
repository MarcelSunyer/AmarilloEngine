#include "GameObject.h"
#include <vector>


GameObject::GameObject()
{
	this->mName = "";
	this->tag = "No Tag";
	parent = nullptr;

	active = true;
	selected = false;

	transform = (ComponentTransform*)AddComponent(ComponentTypes::TRANSFORM);
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
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
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

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		Component* component_enable = *it;
		component_enable->Disable();
	}
}

void GameObject::Update()
{
	for (std::vector<Component*>::iterator co = components.begin(); co != components.end(); ++co)
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
	for (size_t i = 0; i < gameobject->children.size(); ++i)
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
	for (int i = 0; i < children.size(); ++i) {
		if (children[i] == child) {
			children.erase(children.begin() + i);
			child->parent = nullptr;
		}
	}
}

void GameObject::DebugDraw()
{
	for (std::vector<Component*>::iterator co = components.begin(); co != components.end(); ++co)
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


void* GameObject::GetComponentScript(const char* script_class_name)
{
	auto item = components.begin();
	for (; item != components.end(); ++item) {
		if (*item != nullptr && (*item)->type == ComponentTypes::SCRIPT) {
			ComponentScript* script = (ComponentScript*)*item;
			if (TextCmp(script->data_name.data(), script_class_name)) {
				return script->data_ptr;
			}
		}
	}
	return nullptr;
}

const void* GameObject::GetComponentScript(const char* script_class_name) const
{
	auto item = components.cbegin();
	for (; item != components.cend(); ++item) {
		if (*item != nullptr && (*item)->type == ComponentTypes::SCRIPT) {
			ComponentScript* script = (ComponentScript*)*item;
			if (TextCmp(script->data_name.data(), script_class_name)) {
				return script->data_ptr;
			}
		}
	}
	return nullptr;
}

Component* GameObject::GetComponentInChildren(const ComponentTypes type, bool recursive)
{
	auto item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			auto item2 = (*item)->components.begin();
			for (; item2 != (*item)->components.end(); ++item2) {
				if (*item2 != nullptr && (*item2)->type == (ComponentTypes)type) {		//TODO: Revisar, puede dar error	
					return *item2;
				}
				if (recursive) {
					(*item)->GetComponentInChildren(type, true);
				}
			}
		}
	}
	return nullptr;
}

const Component* GameObject::GetComponentInChildren(const ComponentTypes type, bool recursive) const
{
	auto item = children.cbegin();
	for (; item != children.cend(); ++item) {
		if (*item != nullptr) {
			auto item2 = (*item)->components.begin();
			for (; item2 != (*item)->components.end(); ++item2) {
				if (*item2 != nullptr && (*item2)->type == (ComponentTypes)type) {		//TODO: Revisar, puede dar error
					return *item2;
				}
				if (recursive) {
					(*item)->GetComponentInChildren(type, true);
				}
			}
		}
	}
	return nullptr;
}

uint GameObject::GetComponentsInChildren(const ComponentTypes type, Component*** comp_array, bool recursive)
{
	if (children.empty()) {
		return 0u;
	}
	std::vector<Component*> found;
	GetComponentsChildren(type, &found, recursive);

	if (found.empty()) {
		return 0u;
	}

	(*comp_array) = new Component * [found.size()];
	for (uint i = 0; i < found.size(); ++i) {
		(*comp_array)[i] = found[i];
	}
	return found.size();
}

const uint GameObject::GetComponentsInChildren(const ComponentTypes type, Component*** comp_array, bool recursive) const
{
	if (children.empty()) {
		return 0u;
	}
	std::vector<Component*> found;
	GetComponentsChildren(type, &found, recursive);

	if (found.empty()) {
		return 0u;
	}

	(*comp_array) = new Component * [found.size()];
	for (uint i = 0; i < found.size(); ++i) {
		(*comp_array)[i] = found[i];
	}
	return found.size();
}

uint GameObject::GetComponentsInParent(const ComponentTypes type, Component*** comp_array)
{
	if (parent != nullptr && parent->parent != nullptr) { // to know that is not root
		std::vector<Component*> found;
		for (uint i = 0; i < parent->components.size(); ++i) {
			if (parent->components[i] != nullptr && parent->components[i]->type == (ComponentTypes)type) {
				found.push_back(parent->components[i]);
			}
		}
		if (!found.empty()) {
			(*comp_array) = new Component * [found.size()];
			for (uint i = 0; i < found.size(); ++i) {
				(*comp_array)[i] = found[i];
			}
		}
		return found.size();
	}
	return 0u;
}

const uint GameObject::GetComponentsInParent(const ComponentTypes type, Component*** comp_array) const
{
	if (parent != nullptr && parent->parent != nullptr) { // to know that is not root
		std::vector<Component*> found;
		for (uint i = 0; i < parent->components.size(); ++i) {
			if (parent->components[i] != nullptr && parent->components[i]->type == (ComponentTypes)type) {
				found.push_back(parent->components[i]);
			}
		}
		if (!found.empty()) {
			(*comp_array) = new Component * [found.size()];
			for (uint i = 0; i < found.size(); ++i) {
				(*comp_array)[i] = found[i];
			}
		}
		return found.size();
	}
	return 0u;
}

uint GameObject::GetComponentsScript(const char* script_class_name, void*** comp_array)
{
	std::vector<void*> found;
	for (uint i = 0; i < components.size(); ++i) {
		if (components[i] != nullptr && components[i]->type == ComponentTypes::SCRIPT) {
			ComponentScript* script = (ComponentScript*)components[i];
			if (TextCmp(script_class_name, script->data_name.data())) {
				found.push_back(script->data_ptr);
			}
		}
	}
	if (!found.empty()) {
		(*comp_array) = new void* [found.size()];
		for (uint i = 0; i < found.size(); ++i) {
			(*comp_array)[i] = found[i];
		}
	}
	return found.size();
}

const uint GameObject::GetComponentsScript(const char* script_class_name, void*** comp_array) const
{
	std::vector<void*> found;
	for (uint i = 0; i < components.size(); ++i) {
		if (components[i] != nullptr && components[i]->type == ComponentTypes::SCRIPT) {
			ComponentScript* script = (ComponentScript*)components[i];
			if (TextCmp(script_class_name, script->data_name.data())) {
				found.push_back(script->data_ptr);
			}
		}
	}
	if (!found.empty()) {
		(*comp_array) = new void* [found.size()];
		for (uint i = 0; i < found.size(); ++i) {
			(*comp_array)[i] = found[i];
		}
	}
	return found.size();
}

uint GameObject::GetComponentsScriptInChildren(const char* script_class_name, void*** comp_array, bool recursive)
{
	if (children.empty()) {
		return 0u;
	}
	std::vector<void*> found;
	GetComponentsScriptChildren(script_class_name, &found, recursive);

	if (found.empty()) {
		return 0u;
	}

	(*comp_array) = new void* [found.size()];
	for (uint i = 0; i < found.size(); ++i) {
		(*comp_array)[i] = found[i];
	}
	return found.size();
}

const uint GameObject::GetComponentsScriptInChildren(const char* script_class_name, void*** comp_array, bool recursive) const
{
	if (children.empty()) {
		return 0u;
	}
	std::vector<void*> found;
	GetComponentsScriptChildren(script_class_name, &found, recursive);

	if (found.empty()) {
		return 0u;
	}

	(*comp_array) = new void* [found.size()];
	for (uint i = 0; i < found.size(); ++i) {
		(*comp_array)[i] = found[i];
	}
	return found.size();
}

uint GameObject::GetComponentsScriptInParent(const char* script_class_name, void*** comp_array)
{
	if (parent != nullptr && parent->parent != nullptr) {
		std::vector<void*> found;
		for (uint i = 0; i < parent->components.size(); ++i) {
			if (parent->components[i] != nullptr && parent->components[i]->type == ComponentTypes::SCRIPT) {
				ComponentScript* script = (ComponentScript*)parent->components[i];
				if (TextCmp(script_class_name, script->data_name.data())) {
					found.push_back(script->data_ptr);
				}
			}
		}
		if (!found.empty()) {
			(*comp_array) = new void* [found.size()];
			for (uint i = 0; i < found.size(); ++i) {
				(*comp_array)[i] = found[i];
			}
		}
		return found.size();
	}
	return 0u;
}

const uint GameObject::GetComponentsScriptInParent(const char* script_class_name, void*** comp_array) const
{
	if (parent != nullptr && parent->parent != nullptr) {
		std::vector<void*> found;
		for (uint i = 0; i < parent->components.size(); ++i) {
			if (parent->components[i] != nullptr && parent->components[i]->type == ComponentTypes::SCRIPT) {
				ComponentScript* script = (ComponentScript*)parent->components[i];
				if (TextCmp(script_class_name, script->data_name.data())) {
					found.push_back(script->data_ptr);
				}
			}
		}
		if (!found.empty()) {
			(*comp_array) = new void* [found.size()];
			for (uint i = 0; i < found.size(); ++i) {
				(*comp_array)[i] = found[i];
			}
		}
		return found.size();
	}
	return 0u;
}

void* GameObject::GetComponentScriptInParent(const char* script_class_name)
{
	if (parent != nullptr && parent->parent != nullptr) { // parent->parent != nullptr to test is not root :)
		auto item = parent->components.begin();
		for (; item != parent->components.end(); ++item) {
			if (*item != nullptr && (*item)->type == ComponentTypes::SCRIPT) {
				ComponentScript* script = (ComponentScript*)*item;
				if (TextCmp(script->data_name.data(), script_class_name)) {
					return script->data_ptr;
				}
			}
		}
	}
	return nullptr;
}

const void* GameObject::GetComponentScriptInParent(const char* script_class_name) const
{
	if (parent != nullptr && parent->parent != nullptr) { // parent->parent != nullptr to test is not root :)
		auto item = parent->components.cbegin();
		for (; item != parent->components.cend(); ++item) {
			if (*item != nullptr && (*item)->type == ComponentTypes::SCRIPT) {
				ComponentScript* script = (ComponentScript*)*item;
				if (TextCmp(script->data_name.data(), script_class_name)) {
					return script->data_ptr;
				}
			}
		}
	}
	return nullptr;
}

Component* GameObject::GetComponentInParent(const ComponentTypes type)
{
	if (parent != nullptr && parent->parent != nullptr) { // parent->parent != nullptr to test is not root :)
		std::vector<Component*>::iterator item = parent->components.begin();
		for (; item != parent->components.end(); ++item) {
			if (*item != nullptr && (*item)->type == (ComponentTypes)type) {
				return *item;
			}
		}
	}
	return nullptr;
}

const Component* GameObject::GetComponentInParent(const ComponentTypes type) const
{
	if (parent != nullptr && parent->parent != nullptr) { // parent->parent != nullptr to test is not root :)
		std::vector<Component*>::const_iterator item = parent->components.cbegin();
		for (; item != parent->components.cend(); ++item) {
			if (*item != nullptr && (*item)->type == (ComponentTypes)type) {
				return *item;
			}
		}
	}
	return nullptr;
}

void GameObject::GetComponentsChildren(const ComponentTypes type, std::vector<Component*>* to_fill, bool recursive)
{
	auto item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			auto item2 = (*item)->components.begin();
			for (; item2 != (*item)->components.end(); ++item2) {
				if (*item2 != nullptr && (*item2)->type == (ComponentTypes)type) {
					to_fill->push_back(*item2);
				}
			}
			if (recursive) {
				(*item)->GetComponentsChildren(type, to_fill, true);
			}
		}
	}
}

void GameObject::GetComponentsChildren(const ComponentTypes type, std::vector<Component*>* to_fill, bool recursive) const
{
	auto item = children.cbegin();
	for (; item != children.cend(); ++item) {
		if (*item != nullptr) {
			auto item2 = (*item)->components.cbegin();
			for (; item2 != (*item)->components.cend(); ++item2) {
				if (*item2 != nullptr && (*item2)->type == (ComponentTypes)type) {
					to_fill->push_back(*item2);
				}
			}
			if (recursive) {
				(*item)->GetComponentsChildren(type, to_fill, true);
			}
		}
	}
}

void GameObject::GetComponentsScriptChildren(const char* script_calss_name, std::vector<void*>* to_fill, bool recursive)
{
	auto item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			auto item2 = (*item)->components.begin();
			for (; item2 != (*item)->components.end(); ++item2) {
				if (*item2 != nullptr && (*item2)->type == ComponentTypes::SCRIPT) {
					ComponentScript* script = (ComponentScript*)*item2;
					if (TextCmp(script_calss_name, script->data_name.data())) {
						to_fill->push_back(script->data_ptr);
					}
				}
			}
			if (recursive) {
				(*item)->GetComponentsScriptChildren(script_calss_name, to_fill, true);
			}
		}
	}
}

void GameObject::GetComponentsScriptChildren(const char* script_calss_name, std::vector<void*>* to_fill, bool recursive) const
{
	auto item = children.cbegin();
	for (; item != children.cend(); ++item) {
		if (*item != nullptr) {
			auto item2 = (*item)->components.cbegin();
			for (; item2 != (*item)->components.cend(); ++item2) {
				if (*item2 != nullptr && (*item2)->type == ComponentTypes::SCRIPT) {
					ComponentScript* script = (ComponentScript*)*item2;
					if (TextCmp(script_calss_name, script->data_name.data())) {
						to_fill->push_back(script->data_ptr);
					}
				}
			}
			if (recursive) {
				(*item)->GetComponentsScriptChildren(script_calss_name, to_fill, true);
			}
		}
	}
}

uint GameObject::GetComponents(const ComponentTypes type, Component*** comp_array)
{
	std::vector<Component*> found;
	for (uint i = 0; i < components.size(); ++i) {
		if (components[i] != nullptr && components[i]->type == (ComponentTypes)type) {
			found.push_back(components[i]);
		}
	}
	if (!found.empty()) {
		(*comp_array) = new Component * [found.size()];
		for (uint i = 0; i < found.size(); ++i) {
			(*comp_array)[i] = found[i];
		}
	}
	return found.size();
}

const uint GameObject::GetComponents(const ComponentTypes type, Component*** comp_array) const
{
	std::vector<Component*> found;
	for (uint i = 0; i < components.size(); ++i) {
		if (components[i] != nullptr && components[i]->type == (ComponentTypes)type) {
			found.push_back(components[i]);
		}
	}
	if (!found.empty()) {
		(*comp_array) = new Component * [found.size()];
		for (uint i = 0; i < found.size(); ++i) {
			(*comp_array)[i] = found[i];
		}
	}
	return found.size();
}


const char* GameObject::GetTag() const
{
	return tag.c_str();
}