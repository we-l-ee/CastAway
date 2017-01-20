#pragma once
#include "GTiledTextureObject.h"
class Sky :
	public GTiledTextureObject
{
	static GLuint VAO, VBO;
	static unsigned int POINTS_SIZE;
	static Texture DAY,NIGHT;
	
public:
	Sky(const glm::vec3 & displacement);

	void render() override;

	static void initialize();

	~Sky();
};

