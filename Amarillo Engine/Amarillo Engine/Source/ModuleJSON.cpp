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

void JSON_Doc::SetHierarchy(const char* key, const std::vector<GameObject*>& gameObjects)
{
	JSON_Value* hierarchyValue = json_value_init_array();
	JSON_Array* hierarchyArray = json_value_get_array(hierarchyValue);

	for (const auto& gameObject : gameObjects) {

		JSON_Value* gameObjectValue = json_value_init_object();
		JSON_Object* gameObjectObject = json_value_get_object(gameObjectValue);

		// Call the existing SetGameObject function to set individual GameObject properties
		SetGameObject(gameObjectObject, *gameObject);

		// Add the GameObject to the hierarchy array
		json_array_append_value(hierarchyArray, gameObjectValue);
	}

	// Add the hierarchy array to the main object
	json_object_set_value(object, key, hierarchyValue);
}

GameObject* JSON_Doc::GetGameObject(const char* key) const
{
	//JSON_Value* gameObjectValue = json_object_get_value(object, key);

	//if (gameObjectValue != nullptr && json_value_get_type(gameObjectValue) == JSONObject) {
	//	JSON_Object* gameObjectObject = json_value_get_object(gameObjectValue);

	//	// Create a new GameObject
	//	GameObject* gameObject = new GameObject();

	//	// Get Name
	//	const char* name = json_object_get_string(gameObjectObject, "Name");
	//	gameObject->name = (name != nullptr) ? name : "";

	//	// Get UID
	//	gameObject->UID = static_cast<int>(json_object_get_number(gameObjectObject, "UID"));

	//	// Get Parent UID
	//	if (json_object_has_value_of_type(gameObjectObject, "Parent UID", JSONNumber)) {
	//		gameObject->mParent->UID = static_cast<int>(json_object_get_number(gameObjectObject, "Parent UID"));
	//	}

	//	// Get Children UID
	//	if (json_object_has_value_of_type(gameObjectObject, "Children UID", JSONArray)) {
	//		JSON_Array* childrenArray = json_object_get_array(gameObjectObject, "Children UID");
	//		size_t numChildren = json_array_get_count(childrenArray);

	//		for (size_t i = 0; i < numChildren; ++i) {
	//			int childUID = static_cast<int>(json_array_get_number(childrenArray, i));
	//			// You need to find the corresponding child GameObject using the UID
	//			// and add it to gameObject->mChildren.
	//			// Assuming you have a function like FindGameObjectByUID, implement it accordingly.
	//			//gameObject->mChildren.push_back(FindGameObjectByUID(childUID));
	//		}

	//	}

	//	// Get Components
	//	if (json_object_has_value_of_type(gameObjectObject, "Components", JSONArray)) {
	//		JSON_Array* componentsArray = json_object_get_array(gameObjectObject, "Components");
	//		size_t numComponents = json_array_get_count(componentsArray);

	//		for (size_t i = 0; i < numComponents; ++i) {
	//			JSON_Value* componentValue = json_array_get_value(componentsArray, i);

	//			if (json_value_get_type(componentValue) == JSONObject) {
	//				JSON_Object* componentObject = json_value_get_object(componentValue);

	//				// Create a new Component
	//				Component* component = new Component();

	//				// Call the existing GetComponent function to retrieve individual Component properties
	//				GetComponent(componentObject, *component);

	//				// Add the Component to the GameObject's components vector
	//				gameObject->mComponents.push_back(component);
	//			}
	//		}
	//	}

	//	return gameObject;
	//}

	return nullptr;
}

void JSON_Doc::SetComponent(JSON_Object* componentObject, const Component& component)
{
	if (component.type == ComponentTypes::NONE) {

		// Handle NONE case (if needed)

	}
	else if (component.type == ComponentTypes::TRANSFORM) {

		json_object_set_string(componentObject, "Type", "Transform");

		ComponentTransform* transform = (ComponentTransform*)&component;

		json_object_set_number(componentObject, "Active", transform->active);

		// Translation

		JSON_Value* translationArrayValue = json_value_init_array();
		JSON_Array* translationArray = json_value_get_array(translationArrayValue);

		json_array_append_number(translationArray, transform->GetPosition().x);	//Puede dar error @eric
		json_array_append_number(translationArray, transform->GetPosition().y);
		json_array_append_number(translationArray, transform->GetPosition().z);

		json_object_set_value(componentObject, "Translation", translationArrayValue);

		// Rotation

		JSON_Value* rotationArrayValue = json_value_init_array();
		JSON_Array* rotationArray = json_value_get_array(rotationArrayValue);

		json_array_append_number(rotationArray, transform->GetRotation().x);	//Puede dar error @eric
		json_array_append_number(rotationArray, transform->GetRotation().y);
		json_array_append_number(rotationArray, transform->GetRotation().z);

		json_object_set_value(componentObject, "Rotation", rotationArrayValue);

		// Scale

		JSON_Value* scaleArrayValue = json_value_init_array();
		JSON_Array* scaleArray = json_value_get_array(scaleArrayValue);

		json_array_append_number(scaleArray, transform->GetScale().x);
		json_array_append_number(scaleArray, transform->GetScale().y);
		json_array_append_number(scaleArray, transform->GetScale().z);

		json_object_set_value(componentObject, "Scale", scaleArrayValue);

	}
	else if (component.type == ComponentTypes::MESH) {

		json_object_set_string(componentObject, "Type", "Mesh");

		ComponentMesh* mesh = (ComponentMesh*)&component;

		json_object_set_number(componentObject, "Active", mesh->active);

		json_object_set_number(componentObject, "Vertex Count", mesh->mesh_->ourVertex.size());		//Puede dar error @eric
		json_object_set_number(componentObject, "Index Count", mesh->mesh_->indices.size());		//Puede dar error @eric

	}
	else if (component.type == ComponentTypes::TEXTURE) {

		json_object_set_string(componentObject, "Type", "Texture");

		ComponentTexture* texture = (ComponentTexture*)&component;

		json_object_set_number(componentObject, "Active", texture->active);

	}
	else if (component.type == ComponentTypes::CAMERA) {

		json_object_set_string(componentObject, "Type", "Camera");

		ComponentCamera* camera = (ComponentCamera*)&component;

		json_object_set_number(componentObject, "Active", camera->active);

		// Pos (Aqui faltan cosas)

		JSON_Value* posArrayValue = json_value_init_array();
		JSON_Array* posArray = json_value_get_array(posArrayValue);

		json_array_append_number(posArray, camera->camera->GetPosition().x);
		json_array_append_number(posArray, camera->camera->GetPosition().y);
		json_array_append_number(posArray, camera->camera->GetPosition().z);

		json_object_set_value(componentObject, "Position", posArrayValue);

		// FOV

		json_object_set_number(componentObject, "FOV", camera->camera->GetVerticalFOV());

		// Near Plane

		json_object_set_number(componentObject, "Near Plane", camera->camera->GetNearPlaneDistance());

		// Far Plane

		json_object_set_number(componentObject, "Far Plane", camera->camera->GetFarPlaneDistance());

	}

}

void JSON_Doc::DeleteJSON(const std::string& route)
{
	// Check if the file exists before attempting to delete
	if (applic->file_system->FileExists(route.c_str()))
	{
		// Delete the file
		applic->file_system->FileDelete(route.c_str());
		LOG("Deleted File: %s", route);

	}
}

JSON_Doc* JSON_Doc::GetJSON(const std::string& route) {

	JSON_Doc* jsonFile = new JSON_Doc();

	// Load the existing JSON file

	jsonFile->value = json_parse_file(route.c_str());

	// Error handling
	if (!jsonFile->value) {

		LOG("Error: Unable to load JSON file from %s", route.c_str());

		delete jsonFile;

		return nullptr;
	}

	// Get the JSON object from the root value
	jsonFile->object = json_value_get_object(jsonFile->value);

	return jsonFile;
}

void JSON_Doc::GetComponent(const JSON_Object* componentObject, Component& component) const {

	// Get common properties
	std::string type = json_object_get_string(componentObject, "Type");

	if (type == "Transform") {

		component.type = ComponentTypes::TRANSFORM;

	}
	else if (type == "Mesh") {

		component.type = ComponentTypes::MESH;

	}
	else if (type == "Texture") {

		component.type = ComponentTypes::TEXTURE;

	}
	else if (type == "Camera") {

		component.type = ComponentTypes::CAMERA;

	}

}

void JSON_Doc::SetGameObject(JSON_Object* gameObjectObject, const GameObject& gameObject)
{
	//// Set Name
	//json_object_set_string(gameObjectObject, "Name", gameObject.name.c_str());

	//// Set Position
	////JSON_Value* positionValue = json_value_init_array();
	////JSON_Array* positionArray = json_value_get_array(positionValue);
	////json_array_append_number(positionArray, gameObject.mTransform->translation.x);
	////json_array_append_number(positionArray, gameObject.mTransform->translation.y);
	////json_array_append_number(positionArray, gameObject.mTransform->translation.z);
	////json_object_set_value(gameObjectObject, "Position", positionValue);

	//// Set Rotation
	////JSON_Value* rotationValue = json_value_init_array();
	////JSON_Array* rotationArray = json_value_get_array(rotationValue);
	////json_array_append_number(rotationArray, gameObject.mTransform->rotation.x);
	////json_array_append_number(rotationArray, gameObject.mTransform->rotation.y);
	////json_array_append_number(rotationArray, gameObject.mTransform->rotation.z);
	////json_object_set_value(gameObjectObject, "Rotation", rotationValue);

	//// Set Scale
	////JSON_Value* scaleValue = json_value_init_array();
	////JSON_Array* scaleArray = json_value_get_array(scaleValue);
	////json_array_append_number(scaleArray, gameObject.mTransform->scale.x);
	////json_array_append_number(scaleArray, gameObject.mTransform->scale.y);
	////json_array_append_number(scaleArray, gameObject.mTransform->scale.z);
	////json_object_set_value(gameObjectObject, "Scale", scaleValue);

	//// Set UID
	//json_object_set_number(gameObjectObject, "UID", gameObject.UID);

	//// Set Parent UID
	//if (gameObject.mParent != nullptr) {
	//	json_object_set_number(gameObjectObject, "Parent UID", gameObject.mParent->UID);
	//}

	//// Set Children UID
	//std::vector<int> childrenUID;
	//for (const auto& child : gameObject.mChildren) {
	//	childrenUID.push_back(child->UID);
	//}

	//if (!childrenUID.empty()) {
	//	JSON_Value* childrenValue = json_value_init_array();
	//	JSON_Array* childrenArray = json_value_get_array(childrenValue);

	//	for (const auto& childUID : childrenUID) {
	//		json_array_append_number(childrenArray, childUID);
	//	}

	//	json_object_set_value(gameObjectObject, "Children UID", childrenValue);
	//}

	//// Save Components Info

	//JSON_Value* componentsValue = json_value_init_array();
	//JSON_Array* componentsArray = json_value_get_array(componentsValue);

	//for (const auto& component : gameObject.mComponents) {

	//	JSON_Value* componentValue = json_value_init_object();
	//	JSON_Object* componentObject = json_value_get_object(componentValue);

	//	// Call the existing SetGameObject function to set individual GameObject properties
	//	SetComponent(componentObject, *component);

	//	// Add the GameObject to the hierarchy array
	//	json_array_append_value(componentsArray, componentValue);
	//}

	//// Add the hierarchy array to the main object
	//json_object_set_value(gameObjectObject, "Components", componentsValue);

}
