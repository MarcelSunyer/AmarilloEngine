//TODO: Write a script that do things, like moves a GameObject
#ifndef _MOVE_H_
#define _MOVE_H_

#include "../../AmarilloEngine/Source/Amarillo.h"
#include "AmarilloScripts.h"
#include "../../AmarilloEngine/Source/Globals.h"

class ComponentCamera;
class AMARILLO_ENGINE_API Move2 : Amarillo{
public:
	Move2();

	bool Test() override;

};
extern "C" __declspec(dllexport) Move2* CreateMove2();

#endif // !_MOVE_H_