#include "ReflectionMapper.h"



ReflectionMapper::ReflectionMapper():
ReflectionMapper(512,512)
{
}

ReflectionMapper::ReflectionMapper(const unsigned int & width, const unsigned int & height):
	WIDTH(width), HEIGHT(height)
{
	glGenFramebuffers(1, &ref_buffer);
	glGenTextures(1, &ref_texture);
	glBindFramebuffer(GL_FRAMEBUFFER, ref_buffer);

	glBindTexture(GL_TEXTURE_2D, ref_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ref_texture, 0);


	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

#ifdef _DEBUG
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw GException("Framebuffer reflection one not complete!");
	GObject::throwError("ReflectionMapper():\n");
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ReflectionMapper::startMapping()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ref_buffer);
}

void ReflectionMapper::finishMapping()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


ReflectionMapper::~ReflectionMapper()
{
}
