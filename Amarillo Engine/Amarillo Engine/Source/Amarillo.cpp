#include "Amarillo.h"
#include "Application.h"
#include "GameObject.h"

Amarillo::Amarillo()
{
}

Amarillo::~Amarillo()
{
	//App->objects->CancelInvokes(this);	//TODO: Descomentar esto cuando hayas añadido ModuleObjects
}

bool Amarillo::IsScriptEnabled() const
{
	return (enabled == nullptr) ? false : *enabled;
}

void Amarillo::SetScriptEnable(const bool& enable)
{
	if (enable != *enabled) {
		*enabled = enable;
		if (enable) {
			OnEnable();
		}
		else {
			OnDisable();
		}
	}
}

const char* Amarillo::ToString()
{
	return game_object->mName.c_str();
}

bool Amarillo::HasComponent(const ComponentTypes component) const
{
	if (game_object->GetComponent(component) != nullptr)	//TODO: Puede que de error
	{
		return true;
	}
	else
	{
		return false;
	}
}

Component* Amarillo::GetComponent(const ComponentTypes type)
{
	return game_object->GetComponent(type);
}

const Component* Amarillo::GetComponent(const ComponentTypes type) const
{
	return game_object->GetComponent(type);
}

void* Amarillo::GetComponentScript(const char* script_class_name)
{
	return game_object->GetComponentScript(script_class_name);
}

const void* Amarillo::GetComponentScript(const char* script_class_name) const
{
	return game_object->GetComponentScript(script_class_name);
}

Component* Amarillo::GetComponentInParent(const ComponentTypes type)
{
	return game_object->GetComponentInParent(type);
}

const Component* Amarillo::GetComponentInParent(const ComponentTypes type) const
{
	return game_object->GetComponentInParent(type);
}

void* Amarillo::GetComponentScriptInParent(const char* script_class_name)
{
	return game_object->GetComponentScriptInParent(script_class_name);
}

const void* Amarillo::GetComponentScriptInParent(const char* script_class_name) const
{
	return game_object->GetComponentScriptInParent(script_class_name);
}

Component* Amarillo::GetComponentInChildren(const ComponentTypes type, bool recursive)
{
	return game_object->GetComponentInChildren(type, recursive);
}

const Component* Amarillo::GetComponentInChildren(const ComponentTypes type, bool recursive) const
{
	return game_object->GetComponentInChildren(type, recursive);
}

uint Amarillo::GetComponents(const ComponentTypes type, Component*** comp_array)
{
	return game_object->GetComponents(type, comp_array);
}

const uint Amarillo::GetComponents(const ComponentTypes type, Component*** comp_array) const
{
	return game_object->GetComponents(type, comp_array);
}

uint Amarillo::GetComponentsInChildren(const ComponentTypes type, Component*** comp_array, bool recursive)
{
	return game_object->GetComponentsInChildren(type, comp_array, recursive);
}

const uint Amarillo::GetComponentsInChildren(const ComponentTypes type, Component*** comp_array, bool recursive) const
{
	return game_object->GetComponentsInChildren(type, comp_array, recursive);
}

uint Amarillo::GetComponentsInParent(const ComponentTypes type, Component*** comp_array)
{
	return game_object->GetComponentsInParent(type, comp_array);
}

const uint Amarillo::GetComponentsInParent(const ComponentTypes type, Component*** comp_array) const
{
	return game_object->GetComponentsInParent(type, comp_array);
}

uint Amarillo::GetComponentsScript(const char* script_class_name, void*** script_array)
{
	return game_object->GetComponentsScript(script_class_name, script_array);
}

const uint Amarillo::GetComponentsScript(const char* script_class_name, void*** script_array) const
{
	return game_object->GetComponentsScript(script_class_name, script_array);
}

uint Amarillo::GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive)
{
	return game_object->GetComponentsScriptInChildren(script_class_name, script_array, recursive);
}

const uint Amarillo::GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive) const
{
	return game_object->GetComponentsScriptInChildren(script_class_name, script_array, recursive);
}

uint Amarillo::GetComponentsScriptInParent(const char* script_class_name, void*** script_array)
{
	return game_object->GetComponentsScriptInParent(script_class_name, script_array);
}

const uint Amarillo::GetComponentsScriptInParent(const char* script_class_name, void*** script_array) const
{
	return game_object->GetComponentsScriptInParent(script_class_name, script_array);
}

void Amarillo::Destroy(GameObject* obj)
{
	applic->scene->DeleteGameObject(obj);		//TODO: Cambiar este para que lo haga al final del bucle y no de forma inmediata
}

void Amarillo::DestroyInstantly(GameObject* obj)
{
	applic->scene->DeleteGameObject(obj);
}

bool Amarillo::CompareTag(const char* tag_to_compare) const
{
	return strcmp(game_object->GetTag(), tag_to_compare) == 0;
}

void Amarillo::Invoke(std::function<void()> void_no_params_function, float seconds)
{
	if (void_no_params_function != nullptr) {
		//App->objects->AddInvoke(void_no_params_function, seconds, this);	//TODO: Descomentar esto cuando hayas añadido ModuleObjects
	}
}

void Amarillo::CancelInvoke()
{
	//App->objects->CancelInvokes(this);	//TODO: Descomentar esto cuando hayas añadido ModuleObjects
}

void Amarillo::InvokeRepeating(std::function<void()> void_no_params_function, float seconds_to_first_invoke, float seconds_between_each_call)
{
	if (void_no_params_function != nullptr) {
		//App->objects->AddInvokeRepeating(void_no_params_function, seconds_to_first_invoke, seconds_between_each_call, this);	//TODO: Descomentar esto cuando hayas añadido ModuleObjects
	}
}

//bool Amarillo::IsInvoking(std::function<void()> void_no_params_function)
//{
//	return App->objects->IsInvoking(void_no_params_function);
//}

