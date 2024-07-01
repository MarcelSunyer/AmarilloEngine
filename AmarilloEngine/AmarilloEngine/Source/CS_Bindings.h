#pragma once

#include "Globals.h"
#include "Application.h"

#include "GameObject.h"
#include "ResourceMesh.h"


#include "ComponentMesh.h"
#include "ComponentScript.h"
#include "ComponentTransform.h"

#include "ModuleInput.h"
#include "ModuleScene.h"

#include "ModuleInput.h"


#include "GameObject.h"
#include "../External/MathGeoLib/include/Math/float3.h"

template<typename T>
T DECS_CompToComp(MonoObject* obj)
{
	uintptr_t ptr = 0;
	MonoClass* goClass = mono_object_get_class(obj);
	const char* name = mono_class_get_name(goClass);

	mono_field_get_value(obj, mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	return reinterpret_cast<T>(ptr);
}
//------//
MonoObject* Amarillo_Box_Vector(MonoObject* obj, const char* type, bool global)	//Retorna la nueva posici�n del objeto
{
	//TODO: Quitar esto mas adelante, cuando est� arreglado el Transform
	
		
	if (applic == nullptr)
		return nullptr;

	const char* name = mono_class_get_name(mono_object_get_class(obj));

	float3 value;
	ComponentTransform* workTrans = DECS_CompToComp<ComponentTransform*>(obj);

	if (strcmp(type, "POSITION") == 0)
	{
		(global == true) ? value = workTrans->world_matrix.TranslatePart() : value = workTrans->GetPosition();
	}
	else
	{
		(global == true) ? value = workTrans->world_matrix.GetScale() : value = workTrans->GetScale();
	}

	return applic->scripting_module->Float3ToCS(value);
}
MonoObject* Amarillo_Box_Quat(MonoObject* obj, bool global)	//Retorna la nueva rotaci�n del objeto
{
	//TODO: Quitar esto mas adelante, cuando est� arreglado el Transform
	return nullptr;

	if (applic == nullptr)
		return nullptr;

	const char* name = mono_class_get_name(mono_object_get_class(obj));

	Quat value	;
	GameObject* workGO = applic->scripting_module->GameObject_From_CSGO(obj);

	Quat qTmp = Quat::FromEulerXYZ(workGO->transform->GetRotation().x * DEGTORAD, workGO->transform->GetRotation().y * DEGTORAD, workGO->transform->GetRotation().z * DEGTORAD);

	(global == true) ? value = workGO->transform->world_matrix.RotatePart().ToQuat().Normalized() : value = qTmp;


	return applic->scripting_module->QuatToCS(value);
}

#pragma region Internals
//-------------------------------------------- Internals -----------------------------------------------//
void CSLog(MonoString* x)	
{
	if (x == NULL)
		return;

	char* msg = mono_string_to_utf8(x);
	LOG(msg);
	mono_free(msg);
}

int GetKey(MonoObject* x)
{
	if (applic != nullptr)
		return applic->input->GetKey(*(int*)mono_object_unbox(x));

	return 0;
}

int GetMouseClick(MonoObject* x)
{
	if (applic != nullptr)
		return applic->input->GetMouseButton(*(int*)mono_object_unbox(x));

	return 0;
}
int MouseX()
{
	if (applic != nullptr)
		return applic->input->GetMouseXMotion();

	return 0;
}
int MouseY()
{
	if (applic != nullptr)
		return applic->input->GetMouseYMotion();

	return 0;
}

void CSCreateGameObject(MonoObject* name, MonoObject* position)
{
	if (applic == nullptr)
		return;
	//Todo: Crear bala
	char* p = mono_string_to_utf8(mono_object_to_string(name, NULL));
	//GameObject* go = applic->scene->PostUpdateCreateGameObject(p, applic->scene->mRootNode);
	mono_free(p);

	float3 posVector = ModuleScripting::UnboxVector(position);

	//go->mTransform->SetPosition( posVector);
	//go->mTransform->updateTransform = true;	//TODO: No tenemos la variable esta "updateTransform"
}

GameObject* DECS_Comp_To_GameObject(MonoObject* component)
{
	uintptr_t ptr = 0;
	MonoClass* goClass = mono_object_get_class(component);

		mono_field_get_value(component, mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	return reinterpret_cast<Component*>(ptr)->owner;
}
MonoObject* CS_Component_Get_GO(MonoObject* thisRef)
{	
	return applic->scripting_module->GoToCSGO(DECS_Comp_To_GameObject(thisRef));
}
MonoString* Get_GO_Name(MonoObject* go)
{
	if (applic == nullptr)
		return nullptr;

	return mono_string_new(
		applic->scripting_module->domain,
		applic->scripting_module->GameObject_From_CSGO(go)->mName.c_str());
}

MonoObject* SendPosition(MonoObject* obj) //Allows to send float3 as "objects" in C#, should find a [wa]y to move Vector3 as class
{
	//return mono_value_box(External->moduleMono->domain, vecClass, External->moduleMono->Float3ToCS(C_Script::runningScript->GetGO()->transform->position)); //Use this method to send "object" types
	return Amarillo_Box_Vector(obj, "POSITION", false); //Use this method to send class types
}

void RecievePosition(MonoObject* obj, MonoObject* secObj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	if (applic == nullptr)
		return;

	float3 omgItWorks = applic->scripting_module->UnboxVector(secObj);
	ComponentTransform* workTrans = DECS_CompToComp<ComponentTransform*>(obj); //TODO IMPORTANT: First parameter is the object reference, use that to find UID
	if (workTrans)
	{
		workTrans->SetLocalPosition(omgItWorks);
	
	}
}

MonoObject* GetForward(MonoObject* go)	
{
	if (applic == nullptr || CScript::runningScript == nullptr)
		return nullptr;

	GameObject* workGO = applic->scripting_module->GameObject_From_CSGO(go);

	MonoClass* vecClass = mono_class_from_name(applic->scripting_module->image, AMARILLO_SCRIPTS_NAMESPACE, "Vector3");

	//return applic->scripting_module->Float3ToCS(workGO->transform->GetForward());	//TODO: No tenemos GetForward()
}
MonoObject* GetRight(MonoObject* go)
{
	if (applic == nullptr)
		return nullptr;

	GameObject* workGO = applic->scripting_module->GameObject_From_CSGO(go);

	MonoClass* vecClass = mono_class_from_name(applic->scripting_module->image, AMARILLO_SCRIPTS_NAMESPACE, "Vector3");
	//return applic->scripting_module->Float3ToCS(workGO->transform->GetRight());	//TODO: No tenemos GetRight()
}

MonoObject* SendRotation(MonoObject* obj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	return Amarillo_Box_Quat(obj, false); //Use this method to send class types
}

void RecieveRotation(MonoObject* obj, MonoObject* secObj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	if (applic == nullptr)
		return;

	Quat omgItWorks = applic->scripting_module->UnboxQuat(secObj);
	GameObject* workGO = applic->scripting_module->GameObject_From_CSGO(obj); //TODO IMPORTANT: First parameter is the object reference, use that to find UID

	if (workGO->transform)
	{
		//workGO->mTransform->SetPosition(workGO->mTransform->translation, omgItWorks, workGO->mTransform->localScale);
		//workGO->mTransform->updateTransform = true; //TODO: No tenemos la variable esta "updateTransform"
	}
}

MonoObject* SendScale(MonoObject* obj)
{
	return Amarillo_Box_Vector(obj, "SCALE", false);
}


void Destroy(MonoObject* go)
{
	if (go == NULL)
		return;

	MonoClass* klass = mono_object_get_class(go);
	//const char* name = mono_class_get_name(klass);

	GameObject* workGO = applic->scripting_module->GameObject_From_CSGO(go);
	//GameObject* workGO = C_Script::runningScript->GetGO();
	if (workGO == nullptr) {
		LOG("[ERROR] Can't delete GameObject from CS");
		return;
	}

	//workGO->DestroyGameObject(); //Todo: Destroy
}

float GetDT()
{
	//IDK if this would work
	//return applic->GetDT();
	return 1;
}

//TODO: Creat Bullet
void CreateBullet(MonoObject* position, MonoObject* rotation, MonoObject* scale) //TODO: We really need prefabs
{
//	if (applic == nullptr)
//		return /*nullptr*/;
//
//	//GameObject* go = applic->scene->PostUpdateCreateGameObject("Bullet", applic->scene->mRootNode);
//	////go->name = std::to_string(go->UID);
//
//	float3 posVector = ModuleMonoManager::UnboxVector(position);
//	float3 rotQuat = ModuleMonoManager::UnboxVector(rotation);
//	float3 scaleVector = ModuleMonoManager::UnboxVector(scale);
//
//	go->mTransform->SetPosition(posVector);
//	go->mTransform->SetRotation(rotQuat);
//	go->mTransform->SetScale(scaleVector);
//	//go->mTransform->updateTransform = true; //TODO: No temenos esta variable "updateTransform"
//
//	//External->resourceManager->ImportFile("Game/Assets/BakerHouse.fbx");
//
//	ResourceMesh* rMesh = (ResourceMesh*)(External->resourceManager->CreateResourceFromLibrary("Assets/863721484.ymesh", ResourceType::MESH, 863721484));
//	
//	CMesh* cmesh = new CMesh(go);
//	cmesh->rMeshReference = rMesh;
//	go->AddComponent(cmesh);
//
//	CMaterial* cmaterial = new CMaterial(go);
//	cmaterial->shaderPath = SHADER_VS_FS;
//	cmaterial->shader.LoadShader(cmaterial->shaderPath);
//	cmaterial->shaderDirtyFlag = false;
//	go->AddComponent(cmaterial);
//
//	//go->AddComponent(ComponentType::MESH);
//	//meshRenderer = dynamic_cast<CMesh*>(go->GetComponent(ComponentType::MESH));
//
//	//Model("Assets/Primitives/Cube.fbx");
//	/*ResourceMesh* test = dynamic_cast<ResourceMesh*>(External->resourceManager->RequestResource(1753294, "Library/Meshes/1753294.ymesh"));
//	meshRenderer->rMeshReference = test;*/
//
//	//Añade el componente Bullet al gameObject Bullet
//	const char* t = "BH_Bullet";
//	Component* c = nullptr;
//	c = new CScript(go, t);
//	go->AddComponent(c);
//
//	/*return mono_gchandle_get_target(cmp->noGCobject);*/
}

//---------- GLOBAL GETTERS ----------//
MonoObject* SendGlobalPosition(MonoObject* obj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	//return mono_value_box(External->moduleMono->domain, vecClass, External->moduleMono->Float3ToCS(C_Script::runningScript->GetGO()->transform->position)); //Use this method to send "object" types
	return Amarillo_Box_Vector(obj, "POSITION", true); //Use this method to send class types
}

MonoObject* SendGlobalRotation(MonoObject* obj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	//return mono_value_box(External->moduleMono->domain, vecClass, External->moduleMono->Float3ToCS(C_Script::runningScript->GetGO()->transform->position)); //Use this method to send "object" types
	return Amarillo_Box_Quat(obj, true); //Use this method to send class types
}

MonoObject* SendGlobalScale(MonoObject* obj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	//return mono_value_box(External->moduleMono->domain, vecClass, External->moduleMono->Float3ToCS(C_Script::runningScript->GetGO()->transform->position)); //Use this method to send "object" types
	return Amarillo_Box_Vector(obj, "SCALE", true); //Use this method to send class types
}
#pragma endregion