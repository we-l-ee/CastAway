#pragma once
#include "GPointLight.h"

class Seon:
	public GPointLight
{
	static GLuint VAO, VBO, points_size;
	static PointLightData POINTLIGHT;

public:

	Seon(const glm::vec3 & _dis, const glm::vec3 & _color );

	static void initialize();

	~Seon();
};

