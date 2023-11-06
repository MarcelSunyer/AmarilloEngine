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

	if (texture != nullptr)
	{
		if (ImGui::CollapsingHeader("Component Texture"))
		{

			ImGui::Text("Path: %s", texture->path_);
			ImGui::Text("Iamge:");
			//Intento de meter una texw
			ImGui::Image(reinterpret_cast<void*>(static_cast<size_t>(texture->textID)), ImVec2(100, 100));
		}

	}
	else
	{
		/*if (ImGui::CollapsingHeader("Component Texture"))
		{
			ImGui::Text("Path: no path");
			ImGui::Text("Iamge: no image");
		}*/
	}

	
}

void ComponentTexture::SetTexture(Texture* texture)
{
	this->texture = texture;
}

Texture* ComponentTexture::GetTexture()
{
	return this->texture;
}
