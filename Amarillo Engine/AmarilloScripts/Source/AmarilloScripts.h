#ifndef AMARILLO_SCRIPTS
#define AMARILLO_SCRIPTS

//TODO: Include all Components (done), Tools and Helpers (used tools/helpers only)
 
#include "../../Amarillo Engine/Source/GameObject.h"
#include "../../Amarillo Engine/Source/Component.h"
#include "../../Amarillo Engine/Source/ComponentCamera.h"
#include "../../Amarillo Engine/Source/ComponentMesh.h"
#include "../../Amarillo Engine/Source/ComponentTexture.h"
#include "../../Amarillo Engine/Source/ComponentTransform.h"
//#include "../../Amarillo Engine/Source/ComponentScript.h"



#ifdef AMARILLOSCRIPTS_EXPORTS
#define AMARILLO_ENGINE_API __declspec(dllexport)
#else
#define AMARILLO_ENGINE_API __declspec(dllimport)
#endif

// define it next to the CreateClass/StructFunct to be able to use the class/struct
#define AMARILLO_FACTORY extern "C" AMARILLO_ENGINE_API

//INSPECTOR MACROS
#define SHOW_IN_INSPECTOR_AS_INPUT_INT(INT_) ComponentScript::InspectorInputInt(&INT_, #INT_)
#define SHOW_IN_INSPECTOR_AS_DRAGABLE_INT(INT_) ComponentScript::InspectorDragableInt(&INT_, #INT_)
#define SHOW_IN_INSPECTOR_AS_SLIDER_INT(INT_, MIN_, MAX_) ComponentScript::InspectorSliderInt(&INT_, #INT_, MIN_, MAX_)

#define SHOW_IN_INSPECTOR_AS_INPUT_FLOAT(FLOAT_) ComponentScript::InspectorInputFloat(&FLOAT_, #FLOAT_)
#define SHOW_IN_INSPECTOR_AS_DRAGABLE_FLOAT(FLOAT_) ComponentScript::InspectorDragableFloat(&FLOAT_, #FLOAT_)
#define SHOW_IN_INSPECTOR_AS_SLIDER_FLOAT(FLOAT_, MIN_, MAX_) ComponentScript::InspectorSliderFloat(&FLOAT_, #FLOAT_, MIN_, MAX_)

#define SHOW_IN_INSPECTOR_AS_CHECKBOX_BOOL(BOOL_) ComponentScript::InspectorBool(&BOOL_, #BOOL_)

#define SHOW_IN_INSPECTOR_AS_PREFAB(PREFAB_) ComponentScript::InspectorPrefab(&PREFAB_, #PREFAB_)

#define SHOW_IN_INSPECTOR_AS_GAMEOBJECT(GAMEOBJECT_) ComponentScript::InspectorGameObject(&GAMEOBJECT_, #GAMEOBJECT_)
//---


#define VARAIBLE_TO_STRING(VAR_) #VAR_

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);



#endif // !AMARILLO_SCRIPTS
