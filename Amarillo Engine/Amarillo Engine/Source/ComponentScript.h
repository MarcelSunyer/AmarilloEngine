#ifndef COMPONENT_SCRIPT	
#define COMPONENT_SCRIPT

#include "GameObject.h"
#include "Component.h"
#include "../External/Guid/uuid.h"
#include "Functions.h"
#include <vector>
#include <string>

//Forward declaration of Prefab class (TODO)

//struct __declspec(dllexport) InspectorScriptData {
//	friend class ComponentScript;
//	enum DataType {
//		INT,
//		INT2,
//		INT3,
//		FLOAT,
//		FLOAT2,
//		FLOAT3,
//		STRING,
//		BOOL,
//		PREFAB,
//		GAMEOBJECT,
//	};
//
//	enum ShowMode {
//		NONE,
//		INPUT_INT,
//		DRAGABLE_INT,
//		SLIDER_INT,
//		INPUT_FLOAT,
//		DRAGABLE_FLOAT,
//		SLIDER_FLOAT,
//		CHECKBOX,
//	};
//
//	InspectorScriptData(const std::string& variable_name, const DataType& variable_type, void* ptr, const ShowMode& mode) {
//		this->variable_name = variable_name;
//		this->variable_type = variable_type;
//		this->ptr = ptr;
//		this->show_as = mode;
//	}
//
//	std::string variable_name;
//	DataType variable_type;
//	ShowMode show_as;
//	void* ptr = nullptr;
//
//	GameObject** obj = nullptr;
//private:
//	//TODO: Fix this and give valors on the .cpp
//	float min_slider = 0;
//	float max_slider = 0;
//};
//
//class __declspec(dllexport) ComponentScript : public Component {
//	friend class Prefab;
//	friend class ModuleObjects;
//	friend class GameObject;
//public:
//	ComponentScript(GameObject* attach);
//	virtual ~ComponentScript();
//
//private:
//
//	void Enable() override;
//	//void Update() override;	//TODO: No entiendo
//	void Disable() override;
//	void OnEditor() override;
//
//
//	void Reset();
//	void SetComponent(Component* component);
//
//	//TODO: Adapt this to the FileSystem code
//	//void SaveComponent(JSONArraypack* to_save);
//	//void LoadComponent(JSONArraypack* to_load);
//
//	void Clone(Component* clone);
//
//	void LoadData(const char* name, bool is_engine);
//
//	static std::string GetVariableName(const char* ptr_name);
//public:
//
//	static void InspectorInputInt(int* ptr, const char* ptr_name);
//	static void InspectorDragableInt(int* ptr, const char* ptr_name);
//	static void InspectorSliderInt(int* ptr, const char* ptr_name, const int& min_value, const int& max_value);
//
//	static void InspectorInputFloat(float* ptr, const char* ptr_name);
//	static void InspectorDragableFloat(float* ptr, const char* ptr_name);
//	static void InspectorSliderFloat(float* ptr, const char* ptr_name, const float& min_value, const float& max_value);
//
//	static void InspectorBool(bool* ptr, const char* ptr_name);
//
//	static void InspectorPrefab(Prefab* ptr, const char* ptr_name);
//	
//	static void InspectorGameObject(GameObject** ptr, const char* ptr_name);
//
//private:
//
//	//TODO: Asign a valor to the uuid
//	uuids::uuid resourceID;
//
//	std::vector<InspectorScriptData> inspector_variables;
//	bool need_engine = false;
//	std::string data_name;
//	void* data_ptr = nullptr;
//};


#endif // COMPONENT_SCRIPT
