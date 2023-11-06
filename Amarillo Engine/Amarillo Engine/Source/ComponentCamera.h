#pragma once
#include "Component.h"
#include "ModuleCamera3D.h"


class ComponentCamera : public Component {

public:
	ComponentCamera(GameObject* parent);
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	
private:

};
