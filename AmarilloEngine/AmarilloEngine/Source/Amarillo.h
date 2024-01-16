#pragma once


#include <functional>


class GameObject;
class ComponentTransform;

extern "C" __declspec(dllexport) class Amarillo {
public:

	virtual void Update() {};

	GameObject* gameObject;
};
