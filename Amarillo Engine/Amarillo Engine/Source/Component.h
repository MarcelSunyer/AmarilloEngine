#ifndef __COMPONENT_H__
#define __COMPONENT_H__



class GameObject;

enum ComponentTypes
{
	NONE,
	TRANSFORM,
	MESH,
	TEXTURE,
	CAMERA,
};

class Component {
public:
	Component()
	{
		owner = nullptr;
		type = ComponentTypes::NONE;

		active = true;
	}

	Component(GameObject* parent) : active(true), owner(parent), type(ComponentTypes::NONE) {};

	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};
	virtual void OnEditor() {};

	virtual void DebugDraw() {};
	

public:
	ComponentTypes type;
	bool active;
	GameObject* owner;
};


#endif