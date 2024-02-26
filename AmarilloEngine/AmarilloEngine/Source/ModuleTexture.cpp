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
    bool found = textures.find(file_name) != textures.end();
    if (found) 
    {
        return textures[file_name];
    }
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

        Texture* new_texture = new Texture(texture_id, texture_width, texture_height, file_name);
     
        textures[file_name] = new_texture;
      
            // not found
        

        LOG(file_name.c_str());

        return new_texture;
    }
}

Texture* ModuleTexture::LoadTextureEditor(const char* filename)
{
    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    if (!ilLoadImage(filename)) {
        ILenum error = ilGetError();
        LOG("Failed to load texture %s. DevIL Error: %d", filename, error);
        ilDeleteImages(1, &imageID);
        return 0;
    }

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

    glBindTexture(GL_TEXTURE_2D, 0);

    ilDeleteImages(1, &imageID);

    // Crea un nuevo objeto Texture
    Texture* newTexture = new Texture(textureID, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), filename);

    LOG("Loaded texture %s", filename);

    return newTexture;
}

void ModuleTexture::LoadTextureToGameObject(GameObject* texture, std::string textfile)
{
    Texture* tex = LoadTexture(textfile);
    ComponentTexture* texture_component = (ComponentTexture*)texture->GetComponent(ComponentTypes::TEXTURE);
    
    if (texture_component == nullptr)
    {
        texture_component = (ComponentTexture*)texture->AddComponent(ComponentTypes::TEXTURE);
    }
    texture_component->SetTexture(tex);

}

Texture* ModuleTexture::LoadOrGetTexture(const std::string& resolved_path)
{
    bool found = textures.find(resolved_path) != textures.end();
    if (found)
    {
        return textures[resolved_path];
    }

    // Load the texture using the resolved path
    Texture* new_texture = LoadTexture(resolved_path);

    return new_texture;
}

std::string ModuleTexture::ResolveTexturePath(const std::string& modelFilePath, const std::string& textureRelativePath)
{
    namespace fs = std::filesystem;

    // Obtener el directorio del modelo
    fs::path modelPath(modelFilePath);
    fs::path modelDirectory = modelPath.parent_path();

    // Concatenar el directorio del modelo con la ruta relativa de la textura
    fs::path resolvedPath = modelDirectory / fs::path(textureRelativePath);

    // Devolver la ruta como una cadena
    return resolvedPath.string();
}


