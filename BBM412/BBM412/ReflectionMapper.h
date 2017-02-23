#pragma once
#include "GObject.h"

class ReflectionMapper
{
protected:
	GLuint ref_buffer, ref_texture;
	unsigned int WIDTH, HEIGHT;
public:
	ReflectionMapper();
	ReflectionMapper(const unsigned int & width, const unsigned int & height);
	virtual void startMapping();

	virtual glm::vec3 getReflectionNormal() = 0;

	virtual void finishMapping();

	~ReflectionMapper();
};

