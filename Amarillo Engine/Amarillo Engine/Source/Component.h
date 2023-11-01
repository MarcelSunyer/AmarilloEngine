#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "../External/MathGeoLib/include/Math/float3.h"
#include "../External/MathGeoLib/include/Math/float4x4.h"
class GameObject;

enum ComponentTypes
{
	NONE,
	TRANSFORM,
	MESH,
	TEXTURE,
};

class Component {
public:
	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};

	virtual void OnEditor() {};
	Component(GameObject* parent) : active(true), parent(parent), type(ComponentTypes::NONE) {};

public:
	ComponentTypes type;
	bool active;
	GameObject* parent;
};


#endif