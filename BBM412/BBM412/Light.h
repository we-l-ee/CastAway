#pragma once
#include "GameObject.h"

class Light: public virtual GameObject
{
protected:

	GLuint depthMapBuffer, depthMap;
	
	unsigned int SHADOW_WIDTH, SHADOW_HEIGHT;

public:
	Light();

	void setShadow_WH(const unsigned & _width, const unsigned & _height);

	virtual void increaseLightStrength() = 0;
	virtual void decreaseLightStrength() = 0;

	void startMapping();

	virtual glm::mat4 getLightViewMatrix() = 0;
	virtual glm::mat4 getLightProjMatrix() = 0;

	void finishMapping(unsigned int textureUnit);

	virtual void calculateLightening() = 0;

	~Light();
};

