//#ifndef AMARILLO
//#define AMARILLO
#pragma once

#include <functional>

class GameObject;

class __declspec(dllexport) Amarillo {
public:

	virtual bool Test() { return false; };

	GameObject* gameObject = nullptr;
};

//#endif // AMARILLO