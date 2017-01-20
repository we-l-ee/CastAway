#pragma once
#include "GDirecLight.h"
#include "GBasicTextureObject.h"
#include "GExceptions.h"

#include <fstream>
#include <iostream>

class Sun :
	public GBasicTextureObject, public DirecLight
{
	static GLuint VAO, VBO;
	static unsigned int POINTS_SIZE;
	static DirecLightData DIRECLIGHT_DATA;
	static Texture TEXTURE;

	inline void defaultRender();
	inline void wireframeRender();
	inline void reflectionCalculationRender();

	inline void defaultToggleRender(const glm::mat4 & model);
	inline void wireframeToggleRender(const glm::mat4 & model);
	inline void reflectionCalculationToggleRender(const glm::mat4 & model);


public:
	Sun();

	virtual void calculateLightening() override;



	virtual void render() override;
	virtual void toggleRender(const glm::mat4 & model) override;

	static void initialize();

	~Sun();
};

