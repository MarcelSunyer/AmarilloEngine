#include "ComponentScript.h"
#include "../External/ImGui/imgui.h"

#include "ModuleEditor.h"
#include "ModuleScene.h"

#include "GameObject.h"
#include "Application.h"
#include "ComponentTransform.h"

//#include "DEJsonSupport.h"		//TODO: IDK si necesitaremos esto
#include  "../External/mono/metadata/class.h"
#include  "../External/mono/metadata/object.h"
#include  "../External/mono/metadata/debug-helpers.h"

#include "../External/mmgr/mmgr.h"
#include "ModuleScripting.h"

CScript* CScript::runningScript = nullptr;
CScript::CScript(GameObject* _gm, const char* scriptName) : Component(_gm), noGCobject(0), updateMethod(nullptr)
{
	name = scriptName;
	
	LoadScriptData(name.c_str());
}

CScript::~CScript()
{
	if (CScript::runningScript == this)
		CScript::runningScript = nullptr;

	mono_gchandle_free(noGCobject);

	for (unsigned int i = 0; i < fields.size(); i++)
	{
		/*if (fields[i].type == MonoTypeEnum::MONO_TYPE_CLASS && fields[i].fiValue.goValue != nullptr && fields[i].fiValue.goValue->csReferences.size() != 0)
		{
			std::vector<SerializedField*>::iterator ptr = std::find(fields[i].fiValue.goValue->csReferences.begin(), fields[i].fiValue.goValue->csReferences.end(), &fields[i]);
			if (ptr != fields[i].fiValue.goValue->csReferences.end())
				fields[i].fiValue.goValue->csReferences.erase(ptr);

		}*/
	}

	methods.clear();
	fields.clear();
	//name.clear();

}

void CScript::Update()
{
	if (applic->editor->timerState == Timer_State::STOPPED || applic->editor->timerState == Timer_State::PAUSED || updateMethod == nullptr)
		return;


	CScript::runningScript = this;

	MonoObject* exec = nullptr;

	mono_runtime_invoke(updateMethod, mono_gchandle_get_target(noGCobject), NULL, &exec);  //Peta al hacer PLAY en el motor

	if (exec != nullptr)
	{
		if (strcmp(mono_class_get_name(mono_object_get_class(exec)), "NullReferenceException") == 0)
		{
			LOG("[WARNING] Null reference exception detected");
		}
		else
		{
			LOG("[ERROR] Something went wrong");
		}
	}
}

void CScript::ReloadComponent() {

	LoadScriptData(name.c_str());
}


void CScript::OnEditor()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	bool exists = true;

	std::string label = "##" + owner->UID;
	ImGui::Checkbox(label.c_str(), &active);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader((std::string("Script##") + owner->UID).c_str(), &exists, flags))
	{
		ImGui::Text("Script name: %s", name.c_str());
		if (!active) { ImGui::BeginDisabled(); }

		ImGui::Text("Scripting things");

		for (int i = 0; i < fields.size(); i++)
		{
			DropField(fields[i], "_GAMEOBJECT");
		}

		ImGui::Separator();

		for (int i = 0; i < methods.size(); i++)
		{
			ImGui::Text(methods[i].c_str());
		}

		if (!active) { ImGui::EndDisabled(); }
	}

	/*if (!exists) { mOwner->RemoveComponent(this); }*/
}

void CScript::DropField(SerializedField& field, const char* dropType)
{

	const char* fieldName = mono_field_get_name(field.field);
	ImGui::PushID(fieldName);

	ImGui::Text(fieldName);
	ImGui::SameLine();

	switch (field.type)
	{
	case MonoTypeEnum::MONO_TYPE_BOOLEAN:
		mono_field_get_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.bValue);
		if (ImGui::Checkbox(field.displayName.c_str(), &field.fiValue.bValue))
			mono_field_set_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.bValue);
		break;

	case MonoTypeEnum::MONO_TYPE_I4:
		mono_field_get_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.iValue);
		if (ImGui::InputInt(field.displayName.c_str(), &field.fiValue.iValue, 1, 10))
			mono_field_set_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.iValue);
		break;

	case MonoTypeEnum::MONO_TYPE_CLASS:

		if (strcmp(mono_type_get_name(mono_field_get_type(field.field)), "AmarilloEngine.GameObject") != 0)
		{
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "The class %s can't be serialized yet", mono_type_get_name(mono_field_get_type(field.field)));
			break;
		}

		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), (field.fiValue.goValue != nullptr) ? field.fiValue.goValue->mName.c_str() : "this");

		//Hardcodeado para que asigne el GO del objeto del script a todos los campos

		//field.fiValue.goValue = mOwner;
		//SetField(field.field, field.fiValue.goValue);

		//if (ImGui::BeginDragDropTarget())				//TODO: Esto se encarga de hager Drag & Drop el elemento reference
		//{
		///*	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dropType))
		//	{
		//		field.fiValue.goValue = External->editor->hoveredGO;
		//		SetField(field.field, field.fiValue.goValue);
		//	}*/
		//	ImGui::EndDragDropTarget();
		//}
		break;

	case MonoTypeEnum::MONO_TYPE_R4: {
		//float test = 0.f;
		mono_field_get_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.fValue);
		if (ImGui::InputFloat(field.displayName.c_str(), &field.fiValue.fValue, 0.1f))
			mono_field_set_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.fValue);
		break;
	}

	case MonoTypeEnum::MONO_TYPE_STRING:
	{

		MonoString* str = nullptr;
		mono_field_get_value(mono_gchandle_get_target(noGCobject), field.field, &str);

		char* value = mono_string_to_utf8(str);
		strcpy_s(field.fiValue.strValue, value);
		mono_free(value);

		if (ImGui::InputText(field.displayName.c_str(), &field.fiValue.strValue[0], 50))
		{
			str = mono_string_new(applic->scripting_module->domain, field.fiValue.strValue);
			mono_field_set_value(mono_gchandle_get_target(noGCobject), field.field, str);
			//mono_free(str);
		}
		break;
	}

	default:
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), mono_type_get_name(mono_field_get_type(field.field)));
		break;
	}
	//TODO: Update C# field value
	//ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), mono_type_get_name(mono_field_get_type(field.field)));
	ImGui::PopID();
}

void CScript::LoadScriptData(const char* scriptName)
{
	methods.clear();
	fields.clear();

	MonoClass* klass = mono_class_from_name(applic->scripting_module->image, USER_SCRIPTS_NAMESPACE, scriptName);

	if (klass == nullptr)
	{
		LOG("Script %s was deleted and can't be loaded", scriptName);
		name = "Missing script reference";
		return;
	}

	applic->scripting_module->DebugAllMethods(USER_SCRIPTS_NAMESPACE, scriptName, methods);

	noGCobject = mono_gchandle_new(mono_object_new(applic->scripting_module->domain, klass), false);
	mono_runtime_object_init(mono_gchandle_get_target(noGCobject));

	MonoClass* goClass = mono_object_get_class(mono_gchandle_get_target(noGCobject));
	uintptr_t ptr = reinterpret_cast<uintptr_t>(this);
	mono_field_set_value(mono_gchandle_get_target(noGCobject), mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	MonoMethodDesc* mdesc = mono_method_desc_new(":Update", false);
	updateMethod = mono_method_desc_search_in_class(mdesc, klass);
	mono_method_desc_free(mdesc);

	MonoClass* baseClass = mono_class_get_parent(klass);
	if (baseClass != nullptr)
		applic->scripting_module->DebugAllFields(mono_class_get_name(baseClass), fields, mono_gchandle_get_target(noGCobject), this, mono_class_get_namespace(baseClass));

	applic->scripting_module->DebugAllFields(scriptName, fields, mono_gchandle_get_target(noGCobject), this, mono_class_get_namespace(goClass));
}

void CScript::SetField(MonoClassField* field, GameObject* value)
{
	mono_field_set_value(mono_gchandle_get_target(noGCobject), field, applic->scripting_module->GoToCSGO(value));
}
