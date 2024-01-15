#pragma once

#include "Globals.h"
#include "glmath.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"
#include <vector>
#include "Color.h"
#include "../External/Glew/include/glew.h"
#include "Application.h"

typedef unsigned int uint;

class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();

	bool Create(uint width, uint height);
	void Resize(uint width, uint height);
	void Clear(Color clear_color);
	void Bind();
	void Unbind();
	const GLuint GetTexture() const;

private:
	void Destroy();

private:

	GLuint FBO;
	GLuint TCB;
	GLuint RBO;

	int width;
	int height;
};