#ifndef _JSONLOADER_H_
#define _JSONLOADER_H_

#include "Application.h"
#include "Module.h"
#include "Globals.h"
#include "ModuleFileSystem.h"
#include "../External/Parson/parson.h"

#include "../External/MathGeoLib/include/MathBuildConfig.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"
#include "../External/Guid/uuid.h"
#include <uuids.h>

#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ModuleScene.h"


#include <string>
#include <list>

class GameObject;
class JSON_Arraypack;
class JSON_Doc;


class JSON_Doc
{
public:
	
	JSON_Doc();
	JSON_Doc(JSON_Value* value, JSON_Object* object);
	JSON_Doc(std::string path, JSON_Object* object, JSON_Value* value);
	JSON_Doc(const JSON_Doc& doc);
	~JSON_Doc();
	
	void SetString(const std::string& set, const char* str);
	void SetBool(const std::string& set, bool bo);
	void SetNumber(const std::string& set, double nu);
	void SetNumber3(const std::string& set, float3 val);
	void SetNumber4(const std::string& set, float4 val);
	void SetUid(const std::string& set, uuids::uuid val);
	const char* GetString(const std::string& str, const char* defaul = "");
	const bool GetBool(const std::string& bo, bool defaul = false);
	const double GetNumber(const std::string& nu, double defaul = 0);
	const float3 GetNumber3(const std::string& fl, float3 defaul = float3(0, 0, 0));
	const float4 GetNumber4(const std::string& fl, float4 defaul = float4(0, 0, 0, 0));
	uuids::uuid GetUid(const std::string& set);

	void SetArray(const std::string& set);
	void ClearArray(const std::string& arr);
	void RemoveArrayIndex(const std::string& arr, int index);
	void AddStringToArray(const std::string& arr, const char* str);
	void AddBoolToArray(const std::string& arr, bool bo);
	void AddNumberToArray(const std::string& arr, double set);
	void AddSectionToArray(const std::string& arr);
	bool MoveToSectionFromArray(const std::string& arr, int index);
	const int GetArrayCount(const std::string& set) const;
	const char* GetStringFromArray(const std::string& arr, int index);
	const bool GetBoolFromArray(const std::string& arr, int index);
	const double GetNumberFromArray(const std::string& arr, int index);

	bool MoveToSection(const std::string& set);
	void RemoveSection(const std::string& set);
	void MoveToRoot();
	void AddSection(const std::string& set);

	JSON_Doc GetJsonNode();
	JSON_Value* GetValue();

	void Clear();
	void CleanUp();

public:
	bool FindValue(const char* str, json_value_type type);
	bool FindArrayValue(const char* array, int index, json_value_type type);

	// Getters & Setters
	void SetComponent(const char* key, const Component& component);
	void SetGameObject(const char* key, const GameObject& gameObject);

	Component* GetComponent(const char* key) const;
	GameObject* GetGameObject(const char* key) const;
	

	//save Scene 
	void SetComponent(JSON_Object* componentObject, const Component& component);
	

	JSON_Arraypack* GetArray(const std::string& name);

	void StartSave();
	void FinishSave();

	JSON_Arraypack* InitNewArray(const std::string& name);

	//load scene
	void GetComponent(const JSON_Object* componentObject, Component& component) const;	

private:
	std::string path;

	JSON_Value* value = nullptr;
	JSON_Object* object = nullptr;
	JSON_Object* root = nullptr;

	JSON_Value* save_value = nullptr;
	JSON_Object* save_object = nullptr;

	std::vector<JSON_Arraypack*> arrays;
};

class JSON_Arraypack {

public:

	JSON_Arraypack(JSON_Array* arr, JSON_Value* value) { this->arr = arr; this->value = value; }
	~JSON_Arraypack();

	void SetNumber(const std::string& name, const double& number);
	double GetNumber(const std::string& name);

	void SetBoolean(const std::string& name, const bool& boolean);
	bool GetBoolean(const std::string& name);

	void SetColor(const std::string& name, const Color& color);
	Color GetColor(const std::string& name);

	void SetFloat3(const std::string& name, const float3& numbers);
	float3 GetFloat3(const std::string& name);

	void SetQuat(const std::string& name, const Quat& numbers);
	Quat GetQuat(const std::string& name);

	void SetAnotherNode();
	bool GetAnotherNode();
	void GetFirstNode();
	void GetNode(const uint& index);

	uint GetArraySize();

	void SetString(const std::string& name, const std::string& string_parameter);

	const char* GetString(const std::string& name);

	JSON_Arraypack* InitNewArray(const std::string& name);

	JSON_Arraypack* GetArray(const std::string& name);


private:

	JSON_Array* arr = nullptr;
	JSON_Value* value = nullptr;

	std::vector<JSON_Arraypack*> arrays;
	uint index = 0;
};

class ModuleJSON : public Module
{
public:
	ModuleJSON(Application* app, bool start_enabled = true);
	~ModuleJSON();

	bool Awake();
	bool CleanUp();

	JSON_Doc* LoadJSON(const char* path);
	JSON_Doc* CreateJSON();
	void SaveJson(JSON_Doc* json, const char* path);
	void UnloadJSON(JSON_Doc* path);
};

#endif //_JSONLOADER_H_
