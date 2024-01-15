//#ifndef AMARILLO
//#define AMARILLO
#pragma once

#include <functional>

class GameObject;


class __declspec(dllexport) Amarillo {
public:

	virtual void Update() {};

	GameObject* gameObject;
};

//#endif // AMARILLO