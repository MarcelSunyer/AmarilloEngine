#pragma once
#include "Component.h"


class JSON_Doc;
class Camera3D;
class GameObject;

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
