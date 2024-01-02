//#include "Amarillo.h"
//#include "Application.h"
//#include "GameObject.h"
//
//Amarillo::Amarillo()
//{
//}
//
//Amarillo::~Amarillo()
//{
//	App->objects->CancelInvokes(this);
//}
//
//bool Amarillo::IsScriptEnabled() const
//{
//	return (enabled == nullptr) ? false : *enabled;
//}
//
//void Amarillo::SetScriptEnable(const bool& enable)
//{
//	if (enable != *enabled) {
//		*enabled = enable;
//		if (enable) {
//			OnEnable();
//		}
//		else {
//			OnDisable();
//		}
//	}
//}
//
//const char* Amarillo::ToString()
//{
//	return game_object->ToString();
//}
//
//bool Amarillo::HasComponent(const ComponentType& component) const
//{
//	return game_object->HasComponent(component);
//}
//
//Component* Amarillo::GetComponent(const ComponentType& type)
//{
//	return game_object->GetComponent(type);
//}
//
//const Component* Amarillo::GetComponent(const ComponentType& type) const
//{
//	return game_object->GetComponent(type);
//}
//
//void* Amarillo::GetComponentScript(const char* script_class_name)
//{
//	return game_object->GetComponentScript(script_class_name);
//}
//
//const void* Amarillo::GetComponentScript(const char* script_class_name) const
//{
//	return game_object->GetComponentScript(script_class_name);
//}
//
//Component* Amarillo::GetComponentInParent(const ComponentType& type)
//{
//	return game_object->GetComponentInParent(type);
//}
//
//const Component* Amarillo::GetComponentInParent(const ComponentType& type) const
//{
//	return game_object->GetComponentInParent(type);
//}
//
//void* Amarillo::GetComponentScriptInParent(const char* script_class_name)
//{
//	return game_object->GetComponentScriptInParent(script_class_name);
//}
//
//const void* Amarillo::GetComponentScriptInParent(const char* script_class_name) const
//{
//	return game_object->GetComponentScriptInParent(script_class_name);
//}
//
//Component* Amarillo::GetComponentInChildren(const ComponentType& type, bool recursive)
//{
//	return game_object->GetComponentInChildren(type, recursive);
//}
//
//const Component* Amarillo::GetComponentInChildren(const ComponentType& type, bool recursive) const
//{
//	return game_object->GetComponentInChildren(type, recursive);
//}
//
//uint Amarillo::GetComponents(const ComponentType& type, Component*** comp_array)
//{
//	return game_object->GetComponents(type, comp_array);
//}
//
//const uint Amarillo::GetComponents(const ComponentType& type, Component*** comp_array) const
//{
//	return game_object->GetComponents(type, comp_array);
//}
//
//uint Amarillo::GetComponentsInChildren(const ComponentType& type, Component*** comp_array, bool recursive)
//{
//	return game_object->GetComponentsInChildren(type, comp_array, recursive);
//}
//
//const uint Amarillo::GetComponentsInChildren(const ComponentType& type, Component*** comp_array, bool recursive) const
//{
//	return game_object->GetComponentsInChildren(type, comp_array, recursive);
//}
//
//uint Amarillo::GetComponentsInParent(const ComponentType& type, Component*** comp_array)
//{
//	return game_object->GetComponentsInParent(type, comp_array);
//}
//
//const uint Amarillo::GetComponentsInParent(const ComponentType& type, Component*** comp_array) const
//{
//	return game_object->GetComponentsInParent(type, comp_array);
//}
//
//uint Amarillo::GetComponentsScript(const char* script_class_name, void*** script_array)
//{
//	return game_object->GetComponentsScript(script_class_name, script_array);
//}
//
//const uint Amarillo::GetComponentsScript(const char* script_class_name, void*** script_array) const
//{
//	return game_object->GetComponentsScript(script_class_name, script_array);
//}
//
//uint Amarillo::GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive)
//{
//	return game_object->GetComponentsScriptInChildren(script_class_name, script_array, recursive);
//}
//
//const uint Amarillo::GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive) const
//{
//	return game_object->GetComponentsScriptInChildren(script_class_name, script_array, recursive);
//}
//
//uint Amarillo::GetComponentsScriptInParent(const char* script_class_name, void*** script_array)
//{
//	return game_object->GetComponentsScriptInParent(script_class_name, script_array);
//}
//
//const uint Amarillo::GetComponentsScriptInParent(const char* script_class_name, void*** script_array) const
//{
//	return game_object->GetComponentsScriptInParent(script_class_name, script_array);
//}
//
//void Amarillo::Destroy(GameObject* obj)
//{
//	GameObject::Destroy(obj);
//}
//
//void Amarillo::DestroyInstantly(GameObject* obj)
//{
//	GameObject::DestroyInstantly(obj);
//}
//
//bool Amarillo::CompareTag(const char* tag_to_compare) const
//{
//	return strcmp(game_object->GetTag(), tag_to_compare) == 0;
//}
//
//void Amarillo::Invoke(std::function<void()> void_no_params_function, float seconds)
//{
//	if (void_no_params_function != nullptr) {
//		App->objects->AddInvoke(void_no_params_function, seconds, this);
//	}
//}
//
//void Amarillo::CancelInvoke()
//{
//	App->objects->CancelInvokes(this);
//}
//
//void Amarillo::InvokeRepeating(std::function<void()> void_no_params_function, float seconds_to_first_invoke, float seconds_between_each_call)
//{
//	if (void_no_params_function != nullptr) {
//		App->objects->AddInvokeRepeating(void_no_params_function, seconds_to_first_invoke, seconds_between_each_call, this);
//	}
//}
//
////bool Amarillo::IsInvoking(std::function<void()> void_no_params_function)
////{
////	return App->objects->IsInvoking(void_no_params_function);
////}
//
