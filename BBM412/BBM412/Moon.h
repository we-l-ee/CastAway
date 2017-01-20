#pragma once
#include "DirecLight.h"
#include "GBasicTextureLightObject.h"
#include "GBasicLightObject.h"

class Moon :
	public DirecLight, public GBasicTextureObject, public GBasicLightObject
{
	static GLuint VAO, VBO;
	static unsigned int POINTS_SIZE;
	static DirecLightData DIRECLIGHT_DATA;
	static Texture TEXTURE;

	glm::mat4 modelLight;
	glm::mat4 modelMoon;

public:
	Moon();

	virtual void render() override;
	virtual void toggleRender(const glm::mat4 & model) override;

	virtual void calculateLightening() override;

	static void initialize();
#ifdef _DEBUG
	void debugRender() override;
#endif
	~Moon();
};

