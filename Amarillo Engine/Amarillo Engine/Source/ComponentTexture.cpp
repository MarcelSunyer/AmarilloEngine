#pragma once
#include "ComponentTexture.h"
#include "GameObject.h"
#include "Component.h"
#include <string>
#include "ModuleTexture.h" //TODO: NEEDED ONLY BECAUSE OF MESH CLASS-> Turn Mesh Class into stand alone file.
#include "../External/ImGui/imgui.h"

ComponentTexture::ComponentTexture(GameObject* parent) : Component(parent)
{
	type = ComponentTypes::TEXTURE;
};

void ComponentTexture::Enable() {

}

void ComponentTexture::Disable() {

}

void ComponentTexture::Update() {

}

void ComponentTexture::OnEditor() {
	if (texture != NULL)
	{

		if (ImGui::CollapsingHeader("Component Texture"))
		{
			ImGui::Text("Path: %s", texture->path_);

		}
	}
}

void ComponentTexture::SetTexture(Texture* texture)
{


	this->texture = texture;
	
}
