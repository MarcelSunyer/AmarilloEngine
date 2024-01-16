//TODO: Write a script that do things, like moves a GameObject
#ifndef _MOVE_H_
#define _MOVE_H_

#include "../../AmarilloEngine/Source/Amarillo.h"
#include "AmarilloScripts.h"



class AMARILLO_ENGINE_API Move2 : Amarillo{
public:
	Move2();
	
	void Update() override;

public: 

};
AMARILLO_FACTORY Move2* CreateMove2();

#endif // !_MOVE_H_
