#include "Light.h"

Light::Light():SHADOW_WIDTH(1024), SHADOW_HEIGHT(1024)
{
	// Create a depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	// Allocate storage for the texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		SHADOW_WIDTH, SHADOW_HEIGHT,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// Set the default filtering modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set up depth comparison mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	// Set up wrapping modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Create FBO to render depth into
	glGenFramebuffers(1, &depthMapBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapBuffer);
	// Attach the depth texture to it
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
		depthMapBuffer, 0);
	// Disable color rendering as there are no color attachments
	glDrawBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Light::setShadow_WH(const unsigned & _width, const unsigned & _height)
{
	SHADOW_WIDTH = _width;
	SHADOW_HEIGHT = _height;

}

void Light::startMapping()
{
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapBuffer);
	glBindTexture(GL_FRAMEBUFFER, depthMap);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glEnable(GL_POLYGON_OFFSET_FILL);
}

void Light::finishMapping(unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_POLYGON_OFFSET_FILL);
}

Light::~Light()
{
}

