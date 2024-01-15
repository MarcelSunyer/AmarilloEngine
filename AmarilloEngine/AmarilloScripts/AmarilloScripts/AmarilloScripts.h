#ifndef AMARILLO_SCRIPTS
#define AMARILLO_SCRIPTS

//TODO: Include all Components (done), Tools and Helpers (used tools/helpers only)


//#include "../../AmarilloEngine/Source/GameObject.h"

//#include "../../AmarilloEngine/Source/Component.h"
//#include "../../AmarilloEngine/Source/ComponentMesh.h"
//#include "../../AmarilloEngine/Source/ComponentTexture.h"
//#include "../../AmarilloEngine/Source/ComponentTransform.h"
//#include "../../AmarilloEngine/Source/ComponentScript.h"

#include "../../AmarilloEngine/Source/ComponentCamera.h"

//#include "../../AmarilloEngine/Source/ModuleEditor.h"
#include "../../AmarilloEngine/Source/Globals.h"




#ifdef AMARILLOSCRIPTS_EXPORTS
#define AMARILLO_ENGINE_API __declspec(dllexport)
#else
#define AMARILLO_ENGINE_API __declspec(dllimport)
#endif
// define it next to the CreateClass/StructFunct to be able to use the class/struct
#define AMARILLO_FACTORY extern "C" AMARILLO_ENGINE_API


#define VARAIBLE_TO_STRING(VAR_) #VAR_

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);



#endif // !AMARILLO_SCRIPTS
