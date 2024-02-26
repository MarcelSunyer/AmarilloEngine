#ifndef __COMPONENT_H__
#define __COMPONENT_H__



class GameObject;
class JSON_Doc;

enum ComponentTypes
{
	NONE,
	TRANSFORM,
	MESH,
	TEXTURE,
	CAMERA,
	SCRIPT,
};

extern "C" __declspec(dllexport) class Component {
public:
	Component()
	{
		owner = nullptr;
		type = ComponentTypes::NONE;

		active = true;
	}

	Component(GameObject* parent) : active(true), owner(parent), type(ComponentTypes::NONE) {};

	virtual void Enable() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void Disable() {};
	virtual void OnEditor() {};

	virtual void DebugDraw() {};

	virtual void Serialize(JSON_Doc* json) {};

	virtual void OnStartPlay() {};
	

public:
	ComponentTypes type;
	bool active;
	bool not_destroy = true;		//TODO: Adaptar el constructor para que asigne esto
	GameObject* owner;
};
#endif