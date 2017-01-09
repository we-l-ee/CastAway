#pragma once
#include "GBasicTextureObject.h"
class Sky :
	public GBasicTextureObject
{
	static GLuint VAO, VBO;
	static Texture DAY,NIGHT;

public:
	Sky();


	void initialize();

	~Sky();
};

