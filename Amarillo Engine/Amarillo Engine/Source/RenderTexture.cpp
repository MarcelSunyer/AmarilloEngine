
#include "RenderTexture.h"
#include "ModuleWindow.h"

RenderTexture::RenderTexture()
{
	FBO = 0;
	TCB = 0;
	RBO = 0;
	width = 0;
	height = 0;
}

RenderTexture::~RenderTexture()
{
	Destroy();
}

bool RenderTexture::Create(uint width, uint height)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// Create a Texture Attachment (Texture Color Buffer [TCB])
	glGenTextures(1, &TCB);
	glBindTexture(GL_TEXTURE_2D, TCB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Cambiado a GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Cambiado a GL_NEAREST
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TCB, 0);
	// Create a Renderbuffer Attachment
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG("Framebuffer is not complete");
	}
	// Bind the Default Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void RenderTexture::Resize(uint new_width, uint new_height)
{

}

void RenderTexture::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTexture::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTexture::Destroy()
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &TCB);
	glDeleteTextures(1, &RBO);
}

void RenderTexture::Clear(Color clear_color)
{
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

const uint RenderTexture::GetTexture() const
{
	return TCB;
}