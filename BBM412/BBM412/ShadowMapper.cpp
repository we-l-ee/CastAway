#include "ShadowMapper.h"



ShadowMapper::ShadowMapper():
ShadowMapper(1024,1024)
{
}

ShadowMapper::ShadowMapper(const unsigned & _width, const unsigned & _height):
	WIDTH(_width), HEIGHT(_height)
{

	glGenFramebuffers(1, &buffer);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);


#ifdef _DEBUG

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw GException("Framebuffer Shadow Map not complete!");

	GObject::throwError("ShadowMapper::render():\n");

#endif // _DEBUG

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ShadowMapper::startMapping(const GLfloat & offset1, const GLfloat & offset2)
{


	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(offset1, offset2);

#ifdef _DEBUG
	GObject::throwError("ShadowMapper::startMapping():\n");
#endif // _DEBUG

}


void ShadowMapper::finishMapping(unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_POLYGON_OFFSET_FILL);

#ifdef _DEBUG
	GObject::throwError("Light::finishMapping():\n");
#endif // _DEBUG

}

ShadowMapper::~ShadowMapper()
{}