#pragma once
#include "ComponentTexture.h"
#include "GameObject.h"
#include "Component.h"
#include <string>
#include "ModuleTexture.h" //TODO: NEEDED ONLY BECAUSE OF MESH CLASS-> Turn Mesh Class into stand alone file.
#include "ModuleScene.h"
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
			ImGui::Text("Image:");
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

void ComponentTexture::SwapTextures(std::string textfile)
{
	texture->path_ = textfile;

    ILenum image_id;

    ilGenImages(1, &image_id);
    ilBindImage(image_id);

    ILboolean success;
    success = ilLoadImage(textfile.c_str());

    if (success == IL_TRUE) {
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

        GLuint texture_id;
        ILubyte* texture_data = ilGetData();
        GLuint texture_width = ilGetInteger(IL_IMAGE_WIDTH);
        GLuint texture_height = ilGetInteger(IL_IMAGE_HEIGHT);


        glEnable(GL_TEXTURE_2D);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        ilDeleteImages(1, &image_id);


        delete texture;
        Texture* new_texture = new Texture(texture_id, texture_width, texture_height, textfile);

        this->SetTexture(new_texture);

        //App->renderer3D->BindBuffers();

        
    }
}
