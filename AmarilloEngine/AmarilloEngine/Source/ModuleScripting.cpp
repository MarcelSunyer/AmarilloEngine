#include "Globals.h"
#include "Application.h"
#include "ModuleScripting.h"
#include "ModuleRenderer3D.h"

#include "../External/mono/jit/jit.h"
#include "../External/mono/metadata/assembly.h"
#include "../External/mono/metadata/environment.h"
#include "../External/mono/metadata/mono-config.h"
#include "../External/mono/metadata/object.h"
#include "../External/mono/metadata/debug-helpers.h"
#include "../External/mono/metadata/class.h"
#include "../External/mono/metadata/threads.h"

#include "GameObject.h"
#include "ComponentScript.h"
#include "CS_Bindings.h"
#include "CS_Input_Bindings.h"

#include "PhysfsEncapsule.h"
#include "ModuleEditor.h"

#include <iostream>
#include <fstream>  
#include <filesystem>

#include "../External/PugiXML/pugixml.hpp"
//#include "IM_FileSystem.h"
//#include "ImGui/imgui.h"
//#include "WI_TextEditor.h"
//#include "MO_Editor.h"

#include "../External/ImGui/imgui.h"

#pragma comment( lib, "External/mono/libx86/mono-2.0-boehm.lib" )
#pragma comment( lib, "External/mono/libx86/mono-2.0-sgen.lib" )

#include "../External/mmgr/mmgr.h"
#include "../External/MathGeoLib/include/Math/Quat.h"

ModuleScripting::ModuleScripting(Application* app, bool start_enabled) : Module(app, start_enabled), domain(nullptr), domainThread(nullptr), assembly(nullptr), image(nullptr), jitDomain(nullptr)
{
	CMDCompileCS();

	//mono_jit_set_aot_mode(MonoAotMode::MONO_AOT_MODE_HYBRID);
	mono_set_dirs("mono-runtime/lib", "mono-runtime/etc");
	mono_config_parse(NULL);
	jitDomain = mono_jit_init("myapp");

	mono_add_internal_call("AmarilloEngine.Debug::Log", CSLog);
	mono_add_internal_call("AmarilloEngine.AmarilloComponent::get_gameObject", CS_Component_Get_GO);
	mono_add_internal_call("AmarilloEngine.InternalCalls::CreateGameObject", CSCreateGameObject);	
	mono_add_internal_call("AmarilloEngine.InternalCalls::GetGameObjectByName", GetGOByName);

	mono_add_internal_call("AmarilloEngine.GameObject::get_Name", Get_GO_Name);
							
	mono_add_internal_call("AmarilloEngine.InternalCalls::Destroy", Destroy);
							
	mono_add_internal_call("AmarilloEngine.InternalCalls::CreateBullet", CreateBullet);	//TODO: Descomentar cuando esté el CreateBullet()
							

	mono_add_internal_call("AmarilloEngine.GameObject::GetRight", GetRight);
							
	mono_add_internal_call("AmarilloEngine.Transform::GetForward", GetForward);
	mono_add_internal_call("AmarilloEngine.Transform::get_localPosition", SendPosition);
	mono_add_internal_call("AmarilloEngine.Transform::get_globalPosition", SendGlobalPosition);
	mono_add_internal_call("AmarilloEngine.Transform::set_localPosition", RecievePosition);
	mono_add_internal_call("AmarilloEngine.Transform::get_localRotation", SendRotation);
	mono_add_internal_call("AmarilloEngine.Transform::get_globalRotation", SendGlobalRotation);
	mono_add_internal_call("AmarilloEngine.Transform::set_localRotation", RecieveRotation);
							
	mono_add_internal_call("AmarilloEngine.Transform::get_localScale", SendScale);
	mono_add_internal_call("AmarilloEngine.Transform::get_globalScale", SendGlobalScale);
							
#pragma region GamePad		
	mono_add_internal_call("AmarilloEngine.Input::GetKey", GetKey);
	mono_add_internal_call("AmarilloEngine.Input::GetMouseClick", GetMouseClick);
	mono_add_internal_call("AmarilloEngine.Input::GetMouseX", MouseX);
	mono_add_internal_call("AmarilloEngine.Input::GetMouseY", MouseY);
							
	mono_add_internal_call("AmarilloEngine.Time::get_deltaTime", GetDT);



	InitMono();
}

ModuleScripting::~ModuleScripting()
{}

// -----------------------------------------------------------------
bool ModuleScripting::Init()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleScripting::CleanUp()
{
	LOG("Cleaning mono domain");

	//mono_domain_unload(domain);
	mono_jit_cleanup(jitDomain); //Mono cleanup

	return true;
}

void ModuleScripting::OnGUI()
{
	if (ImGui::CollapsingHeader("Mono Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Compile tool: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "MSBuild portable version");

	}
}

void ModuleScripting::ReCompileCS()
{
	if (applic->editor->timerState == Timer_State::RUNNING)
		return;

	//App->scene->SaveScene("Library/Scenes/tmp.des");	//El Miquel lo tiene q marca la ruta de salida

	//App->scene->CleanScene();		//TODO: No tenemos estas funciones
	//App->renderer3D->ClearAllRenderData();

	mono_domain_unload(domain);
	mono_thread_cleanup();

	while (mono_domain_is_unloading(domain) == true)
	{

	}

	CMDCompileCS();
	InitMono();

	for (auto it = App->scene->game_objects.begin(); it != App->scene->game_objects.end(); ++it) {

		if ((*it) != nullptr) {


			CScript* script = (CScript*)(*it)->GetComponent(ComponentTypes::SCRIPT);
			if (script != nullptr) script->ReloadComponent();

		}


	}
}

//ASK: Is this the worst idea ever? TOO SLOW
float3 ModuleScripting::UnboxVector(MonoObject* _obj)
{
	float3 ret;

	MonoClass* klass = mono_object_get_class(_obj);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "x"), &ret.x);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "y"), &ret.y);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "z"), &ret.z);
	return static_cast<float3>(ret);
}
//ASK: Is this the worst idea ever? TOO SLOW
Quat ModuleScripting::UnboxQuat(MonoObject* _obj)
{
	Quat ret;

	MonoClass* klass = mono_object_get_class(_obj);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "x"), &ret.x);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "y"), &ret.y);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "z"), &ret.z);
	mono_field_get_value(_obj, mono_class_get_field_from_name(klass, "w"), &ret.w);
	return ret;
}
void ModuleScripting::DebugAllFields(const char* className, std::vector<SerializedField>& _data, MonoObject* obj, CScript* script, const char* nameSpace)
{
	void* iter = NULL;
	MonoClassField* field;
	MonoClass* klass = mono_class_from_name(mono_assembly_get_image(applic->scripting_module->assembly), nameSpace, className);
	while (field = mono_class_get_fields(klass, &iter))
	{
		if (mono_field_get_flags(field) != 1) //TODO: Hardcoded private = 1, public = 6, static = 22, wtf does this mean?
		{
			SerializedField pushField = SerializedField(field, obj, script);
			//uint32_t test = mono_field_get_flags(field);

			_data.push_back(pushField);
			//LOG(LogType::L_NORMAL, mono_field_full_name(method2));
		}
	}
}
void ModuleScripting::DebugAllMethods(const char* nsName, const char* className, std::vector<std::string>& _data)
{
	void* iter = NULL;
	MonoMethod* method2;
	MonoClass* klass = mono_class_from_name(mono_assembly_get_image(applic->scripting_module->assembly), nsName, className);
	while (method2 = mono_class_get_methods(klass, &iter))
	{
		_data.push_back(mono_method_full_name(method2, 1));
		//LOG(LogType::L_NORMAL, mono_method_full_name(method2, 1));
	}
}

MonoObject* ModuleScripting::GoToCSGO(GameObject* inGo) const
{
	if (inGo == nullptr) {
		LOG("[WARNING] GoTOCSGO inGo doesn't exist");
		return nullptr;
	}
	MonoClass* goClass = mono_class_from_name(image, AMARILLO_SCRIPTS_NAMESPACE, "GameObject");
	uintptr_t goPtr = reinterpret_cast<uintptr_t>(inGo);

	void* args[3];
	args[0] = &inGo->mName;
	args[1] = &goPtr;

	uintptr_t transPTR = reinterpret_cast<uintptr_t>(inGo->transform);
	args[2] = &transPTR;

	MonoMethodDesc* constructorDesc = mono_method_desc_new("AmarilloEngine.GameObject:.ctor(string,uintptr,uintptr)", true);
	MonoMethod* method = mono_method_desc_search_in_class(constructorDesc, goClass);
	MonoObject* goObj = mono_object_new(domain, goClass);
	mono_runtime_invoke(method, goObj, args, NULL);

	mono_method_desc_free(constructorDesc);

	return goObj;
}

MonoObject* ModuleScripting::Float3ToCS(float3& inVec) const
{

	MonoClass* vecClass = mono_class_from_name(image, AMARILLO_SCRIPTS_NAMESPACE, "Vector3");

	MonoObject* vecObject = mono_object_new(domain, vecClass);
	const char* name = mono_class_get_name(mono_object_get_class(vecObject));

	void* args[3];
	args[0] = &inVec.x;
	args[1] = &inVec.y;
	args[2] = &inVec.z;

	MonoMethodDesc* constDesc = mono_method_desc_new("AmarilloEngine.Vector3:.ctor(single,single,single)", true);
	MonoMethod* method = mono_method_desc_search_in_class(constDesc, vecClass);

	mono_runtime_invoke(method, vecObject, args, NULL);

	mono_method_desc_free(constDesc);
	return vecObject;
}

void ModuleScripting::LoadFieldData(SerializedField& _field, MonoObject* _object)
{
	switch (_field.type)
	{
	case MonoTypeEnum::MONO_TYPE_BOOLEAN:
		mono_field_get_value(_object, _field.field, &_field.fiValue.bValue);
		break;

	case MonoTypeEnum::MONO_TYPE_I4:
		mono_field_get_value(_object, _field.field, &_field.fiValue.iValue);
		break;

	case MonoTypeEnum::MONO_TYPE_CLASS:
		_field.fiValue.goValue = nullptr;
		break;

	case MonoTypeEnum::MONO_TYPE_R4:
		mono_field_get_value(_object, _field.field, &_field.fiValue.fValue);
		break;

	case MonoTypeEnum::MONO_TYPE_STRING:
		mono_field_get_value(_object, _field.field, &_field.fiValue.strValue);
		break;

	default:
		mono_field_get_value(_object, _field.field, &_field.fiValue.iValue);
		break;
	}
}

MonoObject* ModuleScripting::QuatToCS(Quat& inVec) const
{

	MonoClass* quadClass = mono_class_from_name(image, AMARILLO_SCRIPTS_NAMESPACE, "Quaternion");
	MonoObject* quatObject = mono_object_new(domain, quadClass);

	void* args[4];
	args[0] = &inVec.x;
	args[1] = &inVec.y;
	args[2] = &inVec.z;
	args[3] = &inVec.w;

	MonoMethodDesc* constDesc = mono_method_desc_new("AmarilloEngine.Quaternion:.ctor(single,single,single,single)", true);
	MonoMethod* method = mono_method_desc_search_in_class(constDesc, quadClass);

	mono_runtime_invoke(method, quatObject, args, NULL);

	mono_method_desc_free(constDesc);
	return quatObject;
}

GameObject* ModuleScripting::GameObject_From_CSGO(MonoObject* goObj)
{
	if (goObj == nullptr)
		return nullptr;
	uintptr_t ptr = 0;
	MonoClass* goClass = mono_class_from_name(image, AMARILLO_SCRIPTS_NAMESPACE, "GameObject");

	mono_field_get_value(goObj, mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	return reinterpret_cast<GameObject*>(ptr);
}

SerializedField::SerializedField() : field(nullptr), parentSC(nullptr)
{
	fiValue.iValue = 0;
}

SerializedField::SerializedField(MonoClassField* _field, MonoObject* _object, CScript* parent) : field(_field)
{
	type = static_cast<MonoTypeEnum>(mono_type_get_type(mono_field_get_type(field)));
	fiValue.iValue = 0;
	parentSC = parent;

	displayName += "##";
	displayName += mono_field_get_name(_field);

	ModuleScripting::LoadFieldData(*this, _object);
}

void ModuleScripting::CreateAssetsScript(const char* localPath)
{
	std::string unnormalizedPath("Assets/");
	unnormalizedPath += localPath;
	unnormalizedPath = PhysfsEncapsule::UnNormalizePath(unnormalizedPath.c_str());

	std::ofstream outfile(unnormalizedPath.c_str());

	std::string className("Assets/");
	className += localPath;
	className = className.substr(className.find_last_of("/") + 1);
	className = className.substr(0, className.find_last_of("."));

	outfile << "using System;" << std::endl << "using AmarilloEngine;" << std::endl << std::endl << "public class " << className.c_str() << " : AmarilloComponent" << std::endl << "{" << std::endl <<
		"	public void Update()" << std::endl << "	{" << std::endl << std::endl << "	}" << std::endl << std::endl << "}";

	outfile.close();

	AddScriptToSLN(unnormalizedPath.c_str());
	ReCompileCS();
}

void ModuleScripting::AddScriptToSLN(const char* scriptLocalPath)
{
	//TODO: El Miquel usa XML, no entiendo como
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("Assembly-CSharp.csproj");

	if (result.status == pugi::xml_parse_status::status_file_not_found)
		assert(false, "XML File not loaded");

	std::string path; // Should be like ../Assets/Scripts/Hola.cs
	path += scriptLocalPath;
	std::string name = path.substr(path.find_last_of("\\"));

	pugi::xml_node whereToAdd = doc.child("Project");
	for (pugi::xml_node panel = whereToAdd.first_child(); panel != nullptr; panel = panel.next_sibling())
	{
		if (strcmp(panel.name(), "ItemGroup") == 0 && strcmp(panel.first_child().name(), "Compile") == 0)
		{
			panel = panel.append_child();
			panel.set_name("Compile");
			pugi::xml_attribute att = panel.append_attribute("Include");
			att.set_value(path.c_str());

			break;
		}
	}

	doc.save_file("Assembly-CSharp.csproj");
}

void ModuleScripting::RemoveScriptFromSLN(const char* scriptLocalPath)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("Assembly-CSharp.csproj");

	if (result.status == pugi::xml_parse_status::status_file_not_found)
		assert(false, "XML File not loaded");

	std::string path; // Should be like ../Assets/Scripts/Hola.cs

	pugi::xml_node whereToRemove = doc.child("Project");
	for (pugi::xml_node panel = whereToRemove.first_child(); panel != nullptr; panel = panel.next_sibling())
	{
		if (strcmp(panel.name(), "ItemGroup") == 0 && strcmp(panel.first_child().name(), "Compile") == 0)
		{
			for (pugi::xml_node toRemove = panel.first_child(); toRemove != nullptr; toRemove = toRemove.next_sibling())
			{
				path = PhysfsEncapsule::NormalizePath(toRemove.attribute("Include").as_string());

				if (strcmp(path.c_str(), scriptLocalPath) == 0)
				{
					panel.remove_child(toRemove);
					break;
				}
			}
		}
	}

	doc.save_file("Assembly-CSharp.csproj");
}


void ModuleScripting::InitMono()
{
	//mono_set_dirs("mono-runtime/lib", "mono-runtime/etc");
	//mono_config_parse(NULL);

	domain = mono_domain_create_appdomain("D1", NULL);
	mono_domain_set(domain, 0);
	domainThread = mono_thread_attach(domain);

	//mono_thread_attach(domain);

	MonoImageOpenStatus sts;
	assembly = mono_assembly_open("ScriptsAssembly-Output/Assembly-CSharp.dll", &sts);
	//assembly = mono_domain_assembly_open(domain, "CSSolution/Assembly-CSharp/Build/Assembly-CSharp.dll");
	if (!assembly)
		LOG("ERROR");

	image = mono_assembly_get_image(assembly);



	const MonoTableInfo* table_info = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
	int rows = mono_table_info_get_rows(table_info);

	MonoClass* _class = nullptr;

	userScripts.clear();
	for (int i = 1; i < rows; i++)
	{
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
		const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
		if (name[0] != '<')
		{
			const char* name_space = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			_class = mono_class_from_name(image, name_space, name);

			if (_class != nullptr && strcmp(mono_class_get_namespace(_class), AMARILLO_SCRIPTS_NAMESPACE) != 0 && !mono_class_is_enum(_class))
			{
				userScripts.push_back(_class);
				LOG("%s", mono_class_get_name(_class));
			}
		}
	}
}