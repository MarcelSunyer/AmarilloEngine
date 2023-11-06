#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include "ComponentTexture.h"

#pragma comment(lib, "External/DevIL/libx86/DevIL.lib")
#pragma comment(lib, "External/DevIL/libx86/ILU.lib")
#pragma comment(lib, "External/DevIL/libx86/ILUT.lib")


ModuleTexture::ModuleTexture(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

bool ModuleTexture::Start()
{
    return true;
}

bool ModuleTexture::CleanUp()
{
    return true;
}

Texture* ModuleTexture::LoadTexture(std::string file_name)
{
    ILenum image_id;

    ilGenImages(1, &image_id);
    ilBindImage(image_id);

    ILboolean success;
    success = ilLoadImage(file_name.c_str());

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


        ComponentTexture* texture_component = (ComponentTexture*)App->scene->root_object->AddComponent(ComponentTypes::TEXTURE);

        Texture* new_texture = new Texture(texture_id, texture_width, texture_height, file_name);

        texture_component->SetTexture(new_texture);

        //App->renderer3D->BindBuffers();
        

        LOG(file_name.c_str());

        return new_texture;
    }
}


