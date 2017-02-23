#pragma once
#include "stddef.hpp"
#include "GObject.h"

class ShadowMapper
{
	const unsigned WIDTH, HEIGHT;
	GLuint buffer, texture;

public:
	ShadowMapper();
	ShadowMapper(const unsigned & _width, const unsigned & _height);

	void startMapping(const GLfloat & offset1, const GLfloat & offset2);

	virtual glm::mat4 getLightViewMatrix() = 0;
	virtual glm::mat4 getLightProjMatrix() = 0;

	void finishMapping(unsigned int textureUnit);

	~ShadowMapper();
};

