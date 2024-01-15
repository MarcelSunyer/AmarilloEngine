#pragma once
#include "Component.h"
#include "ModuleCamera3D.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleJSON.h"

class ComponentCamera : public Component {

public:
	ComponentCamera(GameObject* parent);
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	void Serialize(JSON_Doc* json) override;

	void DebugDraw() override;

public:
	Camera3D* camera = nullptr;
};
