#pragma once
#include "Module.h"
#include "../External/Glew/include/glew.h"
#include "../External/DevIL/include/il.h"
#include "../External/DevIL/include/ilu.h"
#include "../External/DevIL/include/ilut.h"
#include "../External/SDL/include/SDL_opengl.h"
#include <string>
#include <gl/GL.h>
#include <gl/GLU.h>


struct Texture
{
	Texture(GLuint id, uint _width, uint _height)
	{
		textID = id;
		width = _width;
		height = _height;
	}

	GLuint textID;
	uint width;
	uint height;
};
class ModuleTexture : public Module
{
public:

	ModuleTexture(Application* app, bool start_enabled = true);
	~ModuleTexture() {}
	bool Start();
	bool CleanUp();

	bool GenTexture(GLuint* imgData, GLuint width, GLuint height);

	Texture* LoadTexture(std::string textfile);

	std::string texturePath;


};