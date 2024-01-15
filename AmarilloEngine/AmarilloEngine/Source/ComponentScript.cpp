#include "ComponentScript.h"
#include "Application.h"
#include "ResourcePrefab.h"
#include "ModuleScene.h"

#include <vector>
#include <string>

ComponentScript::ComponentScript(GameObject* attach) : Component(attach)
{
	type = ComponentTypes::SCRIPT;
}

ComponentScript::~ComponentScript()
{
	/*if (data_ptr != nullptr) {
		if (need_engine) {
			Amarillo* amarillo = (Amarillo*)data_ptr;
			std::list<Amarillo*>::iterator item = applic->objects->current_scripts.begin();
			for (; item != applic->objects->current_scripts.end(); ++item) {
				if (*item != nullptr && *item == amarillo) {
					applic->objects->current_scripts.erase(item);
					break;
				}
			}
			if (amarillo != nullptr) {
				delete amarillo;
			}
		}
		else {
			void (*Deleter)(void*) = (void (*)(void*))GetProcAddress(applic->scripts_dll, std::string("Destroy" + std::string(data_name)).data());
			Deleter(data_ptr);
		}
	}*/
}

void ComponentScript::Reset()
{
}
void ComponentScript::OnEditor()
{

	if (ImGui::CollapsingHeader("Component Script"))
	{
		strcpy(newName, script_name.c_str());
		if (ImGui::InputText("Script Name", &newName[0], sizeof(newName)))
		{
			script_name = newName;
		}
	}

}

void ComponentScript::OnStartPlay()
{
	CreateScriptInstance();

}


void ComponentScript::Disable()			//TODO: Revisar 
{
	/*if (need_engine && data_ptr != nullptr) {
		try {
			Amarillo* amarillo = (Amarillo*)data_ptr;
			amarillo->OnDisable();
		}
		catch (...)
		{
			try {
				LOG("CODE ERROR IN THE ONDISABLE OF THE SCRIPT: %s", data_name.data());
			}
			catch (...) {
				LOG("UNKNOWN ERROR IN SCRIPTS ONDISABLE");
			}
		}
	}*/
}

void ComponentScript::Enable()			//TODO: Revisar 
{
	/*if (need_engine && data_ptr != nullptr) {
		try {
			Amarillo* amarillo = (Amarillo*)data_ptr;
			amarillo->OnEnable();
		}
		catch (...)
		{
			try {
				LOG("CODE ERROR IN THE ONENABLE OF THE SCRIPT: %s", data_name.data());
			}
			catch (...) {
				LOG("UNKNOWN ERROR IN SCRIPTS ONENABLE");
			}
		}*/
	/*}*/
}

void ComponentScript::Update()
{
	if (amarillo == nullptr)
	{
		return;
	}
	amarillo->Update();
}


//void ComponentScript::InspectorPrefab(Prefab* ptr, const char* ptr_name)					//TODO: Revisar PREFAB
//{
//	if (ptr != nullptr) {
//		std::string name = typeid(*ptr).name();
//		if (!TextCmp(name.data(), "class Prefab"))
//			return;
//
//		std::string variable_name = GetVariableName(ptr_name);
//
//		ComponentScript* script = applic->scene->actual_script_loading;
//		if (script != nullptr) {
//			script->inspector_variables.push_back(InspectorScriptData(variable_name, InspectorScriptData::DataType::PREFAB, (void*)ptr, InspectorScriptData::NONE));
//		}
//	}
//	else {
//		LOG("Prefab variable must not be a pointer!!");
//	}
//}


void ComponentScript::CreateScriptInstance()
{

	void* (*Creator)() = (void* (*)())GetProcAddress(applic->scripts_dll, std::string("CreateMove2").data());

	if (Creator == nullptr)
	{
		LOG("UNKNOWN ERROR IN SCRIPTS CONSTRUCTOR %s", script_name.c_str());
		return;
	}
	
	void* data_ptr = nullptr;

	try 
	{
		data_ptr = Creator();
	}
	catch(...)
	{
		LOG("UNKNOWN ERROR IN SCRIPTS CONSTRUCTOR %s", script_name.c_str());
		return;
	}
	
	amarillo = (Amarillo*)data_ptr;	
	unsigned int juan = sizeof(Amarillo);
	amarillo->gameObject = owner;
	amarillo->Update();
}
