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
#include "ResourceManager.h"


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
MonoObject* Amarillo_Box_Quat(MonoObject* obj, bool global)
{
	if (applic == nullptr)
	{
		return nullptr;  // Si el contexto de la aplicación es nulo, devuelve nulo
	}

	const char* name = mono_class_get_name(mono_object_get_class(obj));  // Obtiene el nombre de la clase del objeto

	Quat value;  // Quaternion para almacenar el resultado
	ComponentTransform* workTrans = DECS_CompToComp<ComponentTransform*>(obj);  // Obtiene un GameObject desde un objeto CSGO	

	// Obtiene la rotación del GameObject y la convierte a quaternion
	Quat qTmp = Quat::FromEulerXYZ(workTrans->GetRotation().x * DEGTORAD,
		workTrans->GetRotation().y * DEGTORAD,
		workTrans->GetRotation().z * DEGTORAD);

	// Decide qué quaternion devolver basado en la bandera global
	(global == true) ? value = workTrans->world_matrix.RotatePart().ToQuat().Normalized() : value = qTmp;

	// Convierte el quaternion a la representación de C#
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
MonoObject* GetGOByName(MonoObject* name)
{
	std::vector<GameObject*> game_objects_copy = applic->scene->game_objects;

	std::string target_name = mono_string_to_utf8(mono_object_to_string(name, NULL));

	for (auto* game_object : game_objects_copy) {
		if (game_object->active && game_object->mName == target_name) {
			// Encontramos el objeto de juego con el nombre solicitado
			return applic->scripting_module->GoToCSGO(game_object);
		}
	}
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

MonoObject* SendPosition(MonoObject* obj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	//return mono_value_box(External->moduleMono->domain, vecClass, External->moduleMono->Float3ToCS(C_Script::runningScript->GetGO()->transform->position)); //Use this method to send "object" types
	return Amarillo_Box_Vector(obj, "POSITION", false); //Use this method to send class types
}

void RecievePosition(MonoObject* obj, MonoObject* secObj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	if (applic == nullptr)
		return;

	float3 omgItWorks = applic->scripting_module->UnboxVector(secObj);
	ComponentTransform* workTrans = DECS_CompToComp<ComponentTransform*>(obj);
	if (workTrans)
	{
		workTrans->SetLocalPosition(omgItWorks);
	
	}
}

MonoObject* GetForward(MonoObject* go)	
{
	if (applic == nullptr || CScript::runningScript == nullptr)
		return nullptr;

	ComponentTransform* workGO = DECS_CompToComp<ComponentTransform*>(go);
	MonoClass* vecClass = mono_class_from_name(applic->scripting_module->image, AMARILLO_SCRIPTS_NAMESPACE, "Vector3");

	return applic->scripting_module->Float3ToCS(workGO->GetForward());	
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

	ComponentTransform* workGO = DECS_CompToComp<ComponentTransform*>(obj);

	Quat currentRotation = workGO->local_rotation;

	Quat newRotation = currentRotation * omgItWorks;

	newRotation.Normalize();

	workGO->SetLocalRotation(newRotation);
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

	applic->scene->root_object->DeleteChild(workGO); 
}

float GetDT()
{
	//IDK if this would work
	return applic->dt;

}

//TODO: Creat Bullet
void CreateBullet(MonoObject* position, MonoObject* rotation, MonoObject* scale) //TODO: We really need prefabs
{
	if (applic == nullptr)
		return /*nullptr*/;

	GameObject* go = applic->scene->LoadMeshAndTexture("../Assets/Models/Bullet.fbx", "../Assets/Textures/Pikachu-Texture.png");


	float3 posVector = applic->scripting_module->UnboxVector(position);
	Quat rotQuat = applic->scripting_module->UnboxQuat(rotation);
	float3 scaleVector = applic->scripting_module->UnboxVector(scale);

	go->transform->SetWorldPosition(posVector);
	go->transform->SetLocalRotation(rotQuat);
	go->transform->SetWorldScale(scaleVector);

	//go->AddComponent(ComponentType::MESH);
	//meshRenderer = dynamic_cast<CMesh*>(go->GetComponent(ComponentType::MESH));

	//Model("Assets/Primitives/Cube.fbx");
	/*ResourceMesh* test = dynamic_cast<ResourceMesh*>(External->resourceManager->RequestResource(1753294, "Library/Meshes/1753294.ymesh"));
	meshRenderer->rMeshReference = test;*/

	//Añade el componente Bullet al gameObject Bullet
	const char* t = "BH_Bullet";
	Component* c = nullptr;
	c = new CScript(go, t);
	go->AddComponent(c);

	/*return mono_gchandle_get_target(cmp->noGCobject);*/
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