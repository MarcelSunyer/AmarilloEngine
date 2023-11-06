#pragma once
#include "Component.h"
#include "glmath.h"
#include "Globals.h"
#include <string>

struct Texture;


class ComponentTexture : public Component {


public:
	Texture* texture = nullptr;


public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	void SetTexture(Texture* texture);
	Texture* GetTexture();
	void SwapTextures(std::string textfile);

	ComponentTexture(GameObject* parent);
};