#pragma once
#include "Component.h"
#include "glmath.h"
#include "Globals.h"

struct Texture;


class ComponentTexture : public Component {


public:
	Texture* texture;


public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	void SetTexture(Texture* texture);

	ComponentTexture(GameObject* parent);
};