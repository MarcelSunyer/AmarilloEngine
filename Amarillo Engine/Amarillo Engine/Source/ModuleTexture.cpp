#include "Application.h"
#include "ModuleTexture.h"

#include <gl/GL.h>
#include <gl/GLU.h>


#pragma comment(lib, "External/DevIL/libx86/DevIL.lib")
#pragma comment(lib, "External/DevIL/libx86/ILU.lib")
#pragma comment(lib, "External/DevIL/libx86/ILUT.lib")

//#include "SDL/include/SDL_opengl.h"

ModuleTexture::ModuleTexture(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    //textureID = 0;
}

bool ModuleTexture::Start()
{





    return true;
}

bool ModuleTexture::CleanUp()
{
    //glDeleteBuffers(1, &textureID);
    return true;
}

Texture* ModuleTexture::LoadTexture(std::string textfile)
{
    ILenum imageToTextID;
    ILboolean done;

    ilGenImages(1, &imageToTextID);
    ilBindImage(imageToTextID);

    done = ilLoadImage(textfile.c_str());

    if (done == IL_TRUE) {
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

        GLuint _texture;

        GLuint width = ilGetInteger(IL_IMAGE_WIDTH);
        GLuint height = ilGetInteger(IL_IMAGE_HEIGHT);
        ILubyte* textdata = ilGetData();

        glEnable(GL_TEXTURE_2D);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textdata);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        ilDeleteImages(1, &imageToTextID);

        LOG(textfile.c_str());

        return new Texture(_texture, width, height);
    }
}

