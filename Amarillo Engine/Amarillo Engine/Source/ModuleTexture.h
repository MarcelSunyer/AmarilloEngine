#pragma once
#include "Module.h"
#include "../External/Glew/include/glew.h"
#include "../External/DevIL/include/il.h"
#include "../External/DevIL/include/ilu.h"
#include "../External/DevIL/include/ilut.h"
#include "../External/SDL/include/SDL_opengl.h"
#include <string>
#include "GameObject.h"
#include <gl/GL.h>
#include <map>
#include <gl/GLU.h>


struct Texture
{
	Texture()
	{

	}

	Texture(GLuint id, uint _width, uint _height, std::string path)
	{
		textID = id;
		width = _width;
		height = _height;
		path_ = path;
	}

	GLuint textID;
	uint width;
	uint height;
	std::string path_;
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
	GLuint LoadTexture(const char* filename);

	void LoadTextureToGameObject(GameObject* texture, std::string textfile);
	Texture* LoadOrGetTexture(const std::string& resolved_path);

	std::string ResolveTexturePath(const std::string& modelFilePath, const std::string& textureRelativePath);

	std::map<std::string, Texture*> textures;

	

};