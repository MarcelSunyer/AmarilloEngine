#include "ModuleJSON.h"
#include "Globals.h"

#include <string>
#include <list>

ModuleJSON::ModuleJSON(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleJSON::~ModuleJSON()
{
}

bool ModuleJSON::Awake()
{
	bool ret = true;

	LOG("Loading JSON Module");

	return ret;
}

JSON_Doc ModuleJSON::LoadJSON(const char* path)
{
	JSON_Doc ret;

	bool exists = false;
	for (std::list<JSON_Doc>::iterator it = jsons.begin(); it != jsons.end(); it++)
	{
		if (App->file_system->TextCmp(path, (*it).GetPath().c_str()))
		{
			ret = (*it);
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		JSON_Value* user_data = json_parse_file(path);
		JSON_Object* root_object = json_value_get_object(user_data);

		if (user_data == nullptr || root_object == nullptr)
		{
			LOG("Error loading %s", path);
		}
		else
		{
			JSON_Doc new_doc = JSON_Doc(user_data, root_object, path);
			jsons.push_back(new_doc);

			ret = new_doc;
		}
	}

	return ret;
}


JSON_Doc ModuleJSON::CreateJSON(const char* path, const char* name, const char* extension)
{
	JSON_Doc ret;

	std::string filepath = std::string(path) + std::string(name) + std::string(".") + std::string(extension);

	ret = CreateJSON(filepath.c_str());

	return ret;
}

JSON_Doc ModuleJSON::CreateJSON(const char* path)
{
	JSON_Doc ret;

	bool exists = false;
	for (std::list<JSON_Doc>::iterator it = jsons.begin(); it != jsons.end(); it++)
	{
		if (App->file_system->TextCmp(path, (*it).GetPath().c_str()))
		{
			exists = true;
			break;
		}
	}

	if (exists)
	{
		LOG("Error creating %s. There is already a file with this path/name", path);
	}
	else
	{
		JSON_Value* root_value = json_value_init_object();

		if (root_value == nullptr)
		{
			LOG("Error creating %s. Wrong path?", path);
		}
		else
		{
			JSON_Object* root_object = json_value_get_object(root_value);

			JSON_Doc new_doc = JSON_Doc(root_value, root_object, path);
			jsons.push_back(new_doc);

			new_doc.Save();

			ret = new_doc;
		}
	}

	return ret;
}

void ModuleJSON::UnloadJSON(JSON_Doc son)
{
	for (std::list<JSON_Doc>::iterator it = jsons.begin(); it != jsons.end(); ++it)
	{
		if (App->file_system->TextCmp(son.GetPath().c_str(), (*it).GetPath().c_str()))
		{
			(*it).CleanUp();
			it = jsons.erase(it);
			break;
		}
	}
}

bool ModuleJSON::CleanUp()
{
	LOG("Unloading JSON Module");

	for (std::list<JSON_Doc>::iterator it = jsons.begin(); it != jsons.end();)
	{
		(*it).CleanUp();
	}

	jsons.clear();

	return true;
}

JSON_Doc::JSON_Doc()
{
}

JSON_Doc::JSON_Doc(JSON_Value* _value, JSON_Object* _object, const char* _path)
{
	value = _value;
	object = _object;
	root = _object;
	path = _path;
}

JSON_Doc::JSON_Doc(const JSON_Doc& doc)
{
	value = doc.value;
	object = doc.object;
	path = doc.path;
	root = object;
}

JSON_Doc::~JSON_Doc()
{
}

void JSON_Doc::SetString(const std::string& set, const char* str)
{
	json_object_dotset_string(object, set.c_str(), str);
}

void JSON_Doc::SetBool(const std::string& set, bool bo)
{
	json_object_dotset_boolean(object, set.c_str(), bo);
}

void JSON_Doc::SetNumber(const std::string& set, double nu)
{
	json_object_dotset_number(object, set.c_str(), nu);
}

void JSON_Doc::SetNumber3(const std::string& set, float3 val)
{
	SetArray(set);
	AddNumberToArray(set, val.x);
	AddNumberToArray(set, val.y);
	AddNumberToArray(set, val.z);
}

void JSON_Doc::SetNumber4(const std::string& set, float4 val)
{
	SetArray(set);
	AddNumberToArray(set, val.x);
	AddNumberToArray(set, val.y);
	AddNumberToArray(set, val.w);
	AddNumberToArray(set, val.z);
}

void JSON_Doc::SetUid(const std::string& set, uuids::uuid val)
{
	std::string string = uuids::to_string<char>(val);
	SetString(set, string.c_str());
}

void JSON_Doc::SetArray(const std::string& set)
{
	JSON_Value* va = json_value_init_array();
	JSON_Array* arr = json_value_get_array(va);

	json_object_dotset_value(object, set.c_str(), va);
}

void JSON_Doc::ClearArray(const std::string& arr)
{
	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		json_array_clear(array);
	}
}

void JSON_Doc::RemoveArrayIndex(const std::string& arr, int index)
{
	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		json_array_remove(array, index);
	}
}

const int JSON_Doc::GetArrayCount(const std::string& set) const
{
	int ret = 0;

	JSON_Array* array = json_object_get_array(object, set.c_str());

	if (array != nullptr)
	{
		ret = json_array_get_count(array);
	}

	return ret;
}

const char* JSON_Doc::GetStringFromArray(const std::string& arr, int index)
{
	const char* ret = nullptr;

	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		if (FindArrayValue(arr.c_str(), index, json_value_type::JSONString))
		{
			ret = json_array_get_string(array, index);
		}
	}

	return ret;
}

const bool JSON_Doc::GetBoolFromArray(const std::string& arr, int index)
{
	bool ret = false;

	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		if (FindArrayValue(arr.c_str(), index, json_value_type::JSONBoolean))
		{
			ret = json_array_get_boolean(array, index);
		}
	}

	return ret;
}

const double JSON_Doc::GetNumberFromArray(const std::string& arr, int index)
{
	double ret = 0;

	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		if (FindArrayValue(arr.c_str(), index, json_value_type::JSONNumber))
		{
			ret = json_array_get_number(array, index);
		}
	}

	return ret;
}

void JSON_Doc::AddStringToArray(const std::string& arr, const char* str)
{
	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		json_array_append_string(array, str);
	}
}

void JSON_Doc::AddBoolToArray(const std::string& arr, bool bo)
{
	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		json_array_append_boolean(array, bo);
	}
}

void JSON_Doc::AddNumberToArray(const std::string& arr, double set)
{
	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		json_array_append_number(array, set);
	}
}

void JSON_Doc::AddSectionToArray(const std::string& arr)
{
	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		json_array_append_value(array, json_value_init_object());
	}
}

bool JSON_Doc::MoveToSectionFromArray(const std::string& arr, int index)
{
	bool ret = false;

	JSON_Array* array = json_object_get_array(object, arr.c_str());

	if (array != nullptr)
	{
		JSON_Object* obj = json_array_get_object(array, index);

		object = obj;
		ret = true;
	}

	return ret;
}

const char* JSON_Doc::GetString(const std::string& str, const char* defaul)
{
	const char* ret = defaul;

	if (FindValue(str.c_str(), json_value_type::JSONString))
		ret = json_object_dotget_string(object, str.c_str());

	return ret;
}

const bool JSON_Doc::GetBool(const std::string& str, bool defaul)
{
	bool ret = defaul;

	if (FindValue(str.c_str(), json_value_type::JSONBoolean))
		ret = json_object_dotget_boolean(object, str.c_str());

	return ret;
}

const double JSON_Doc::GetNumber(const std::string& str, double defaul)
{
	double ret = defaul;

	if (FindValue(str.c_str(), json_value_type::JSONNumber))
		ret = json_object_dotget_number(object, str.c_str());

	return ret;
}

const float3 JSON_Doc::GetNumber3(const std::string& fl, float3 defaul)
{
	float3 ret = float3::zero;

	ret = defaul;

	JSON_Array* array = json_object_get_array(object, fl.c_str());

	if (array != nullptr)
	{
		ret.x = GetNumberFromArray(fl, 0);
		ret.y = GetNumberFromArray(fl, 1);
		ret.z = GetNumberFromArray(fl, 2);
	}

	return ret;
}

const float4 JSON_Doc::GetNumber4(const std::string& fl, float4 defaul)
{
	float4 ret = float4::zero;

	ret = defaul;

	JSON_Array* array = json_object_get_array(object, fl.c_str());

	if (array != nullptr)
	{
		ret.x = GetNumberFromArray(fl, 0);
		ret.y = GetNumberFromArray(fl, 1);
		ret.w = GetNumberFromArray(fl, 2);
		ret.z = GetNumberFromArray(fl, 3);
	}

	return ret;
}

bool JSON_Doc::MoveToSection(const std::string& set)
{
	bool ret = false;

	JSON_Object* obj = json_object_get_object(object, set.c_str());

	if (obj != nullptr)
	{
		object = obj;
		ret = true;
	}

	return ret;
}

void JSON_Doc::RemoveSection(const std::string& set)
{
	json_object_remove(object, set.c_str());
}

void JSON_Doc::MoveToRoot()
{
	object = root;
}

void JSON_Doc::AddSection(const std::string& set)
{
	json_object_set_value(object, set.c_str(), json_value_init_object());
}

JSON_Doc JSON_Doc::GetJsonNode()
{
	return JSON_Doc(*this);
}

void JSON_Doc::Clear()
{
	json_value_free(value);
	value = json_value_init_object();
	object = json_value_get_object(value);
	root = object;
}

std::string JSON_Doc::GetPath()
{
	return path;
}

void JSON_Doc::Save()
{
	json_serialize_to_file_pretty(value, path.c_str());
}

void JSON_Doc::CleanUp()
{
	json_value_free(value);
}

bool JSON_Doc::FindValue(const char* str, json_value_type type)
{
	bool ret = false;

	JSON_Value* val = json_object_dotget_value(object, str);

	if (val != nullptr && json_value_get_type(val) == type)
		ret = true;

	return ret;
}

bool JSON_Doc::FindArrayValue(const char* arr, int index, json_value_type type)
{
	bool ret = false;

	JSON_Array* array = json_object_get_array(object, arr);

	if (array != nullptr)
	{
		JSON_Value* val = json_array_get_value(array, index);

		if (val != nullptr && json_value_get_type(val) == type)
			ret = true;
	}

	return ret;
}

//void JSON_Doc::SetHierarchy(const char* key, const std::vector<GameObject*>& gameObjects)
//{
//	JSON_Value* hierarchyValue = json_value_init_array();
//	JSON_Array* hierarchyArray = json_value_get_array(hierarchyValue);
//
//	for (const auto& gameObject : gameObjects) {
//
//		JSON_Value* gameObjectValue = json_value_init_object();
//		JSON_Object* gameObjectObject = json_value_get_object(gameObjectValue);
//
//		// Call the existing SetGameObject function to set individual GameObject properties
//		SetGameObject(gameObjectObject, *gameObject);
//
//		// Add the GameObject to the hierarchy array
//		json_array_append_value(hierarchyArray, gameObjectValue);
//	}
//
//	// Add the hierarchy array to the main object
//	json_object_set_value(object, key, hierarchyValue);
//}
//
//void JSON_Doc::SetGameObject(JSON_Object* gameObjectObject, const GameObject& gameObject)
//{
//	// Set Name
//	json_object_set_string(gameObjectObject, "Name", gameObject.mName.c_str());
//
//	// Set UID - TODO: Add UID to gameobjects
//	//json_object_set_number(gameObjectObject, "UID", gameObject.UID);
//
//	// Set Parent UID
//	if (gameObject.parent != nullptr) {
//		//json_object_set_number(gameObjectObject, "Parent UID", gameObject.parent->UID);	//TODO: Add UID to gameobjects
//	}
//
//	// Set Children UID
//	std::vector<int> childrenUID;
//	for (const auto& child : gameObject.children) {
//		//childrenUID.push_back(child->UID);	//TODO: Add UID to gameobjects
//	}
//
//	if (!childrenUID.empty()) {
//		JSON_Value* childrenValue = json_value_init_array();
//		JSON_Array* childrenArray = json_value_get_array(childrenValue);
//
//		for (const auto& childUID : childrenUID) {
//			json_array_append_number(childrenArray, childUID);
//		}
//
//		json_object_set_value(gameObjectObject, "Children UID", childrenValue);
//	}
//
//	// Save Components Info
//
//	JSON_Value* componentsValue = json_value_init_array();
//	JSON_Array* componentsArray = json_value_get_array(componentsValue);
//
//	for (const auto& component : gameObject.components) {
//
//		JSON_Value* componentValue = json_value_init_object();
//		JSON_Object* componentObject = json_value_get_object(componentValue);
//
//		// Call the existing SetGameObject function to set individual GameObject properties
//		SetComponent(componentObject, *component);
//
//		// Add the GameObject to the hierarchy array
//		json_array_append_value(componentsArray, componentValue);
//	}
//
//	// Add the hierarchy array to the main object
//	json_object_set_value(gameObjectObject, "Components", componentsValue);
//
//}
//
//void JSON_Doc::SetComponent(JSON_Object* componentObject, const Component& component)
//{
//	if (component.type == ComponentTypes::NONE) {
//
//		// Handle NONE case (if needed)
//
//	}
//	else if (component.type == ComponentTypes::TRANSFORM) {
//
//		json_object_set_string(componentObject, "Type", "Transform");
//
//		ComponentTransform* transform = (ComponentTransform*)&component;
//
//		json_object_set_number(componentObject, "Active", transform->active);
//
//		// Translation
//
//		JSON_Value* translationArrayValue = json_value_init_array();
//		JSON_Array* translationArray = json_value_get_array(translationArrayValue);
//
//		json_array_append_number(translationArray, transform->GetPosition().x);	//Puede dar error @eric
//		json_array_append_number(translationArray, transform->GetPosition().y);
//		json_array_append_number(translationArray, transform->GetPosition().z);
//
//		json_object_set_value(componentObject, "Translation", translationArrayValue);
//
//		// Rotation
//
//		JSON_Value* rotationArrayValue = json_value_init_array();
//		JSON_Array* rotationArray = json_value_get_array(rotationArrayValue);
//
//		json_array_append_number(rotationArray, transform->GetRotation().x);	//Puede dar error @eric
//		json_array_append_number(rotationArray, transform->GetRotation().y);
//		json_array_append_number(rotationArray, transform->GetRotation().z);
//
//		json_object_set_value(componentObject, "Rotation", rotationArrayValue);
//
//		// Scale
//
//		JSON_Value* scaleArrayValue = json_value_init_array();
//		JSON_Array* scaleArray = json_value_get_array(scaleArrayValue);
//
//		json_array_append_number(scaleArray, transform->GetScale().x);
//		json_array_append_number(scaleArray, transform->GetScale().y);
//		json_array_append_number(scaleArray, transform->GetScale().z);
//
//		json_object_set_value(componentObject, "Scale", scaleArrayValue);
//
//	}
//	else if (component.type == ComponentTypes::MESH) {
//
//		json_object_set_string(componentObject, "Type", "Mesh");
//
//		ComponentMesh* mesh = (ComponentMesh*)&component;
//
//		json_object_set_number(componentObject, "Active", mesh->active);
//
//		json_object_set_number(componentObject, "Vertex Count", mesh->mesh_->ourVertex.size());		//Puede dar error @eric
//		json_object_set_number(componentObject, "Index Count", mesh->mesh_->indices.size());		//Puede dar error @eric
//
//	}
//	else if (component.type == ComponentTypes::TEXTURE) {
//
//		json_object_set_string(componentObject, "Type", "Material");
//
//		ComponentTexture* texture = (ComponentTexture*)&component;
//
//		json_object_set_number(componentObject, "Active", texture->active);
//
//	}
//	else if (component.type == ComponentTypes::CAMERA) {
//
//		json_object_set_string(componentObject, "Type", "Camera");
//
//		ComponentCamera* camera = (ComponentCamera*)&component;
//
//		json_object_set_number(componentObject, "Active", camera->active);
//
//		// Pos (Aqui faltan cosas)
//
//		JSON_Value* posArrayValue = json_value_init_array();
//		JSON_Array* posArray = json_value_get_array(posArrayValue);
//
//		json_array_append_number(posArray, camera->camera->GetPosition().x);
//		json_array_append_number(posArray, camera->camera->GetPosition().y);
//		json_array_append_number(posArray, camera->camera->GetPosition().z);
//
//		json_object_set_value(componentObject, "Position", posArrayValue);
//
//		// FOV
//
//		json_object_set_number(componentObject, "FOV", camera->camera->GetVerticalFOV());
//
//		// Near Plane
//
//		json_object_set_number(componentObject, "Near Plane", camera->camera->GetNearPlaneDistance());
//
//		// Far Plane
//
//		json_object_set_number(componentObject, "Far Plane", camera->camera->GetFarPlaneDistance());
//
//	}
//
//}
