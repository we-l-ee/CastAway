#pragma once
#include "GBasicTextureObject.h"


class GBasicTextureLightObject :
	public GBasicTextureObject
{


protected:
	static GLuint program;

public:




	GBasicTextureLightObject(const string & obj, const GLuint & _prog = program);
	GBasicTextureLightObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog = program);
	GBasicTextureLightObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog = program);

	GBasicTextureLightObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const Texture texture,
		const GLuint & _prog = program);
	GBasicTextureLightObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const Texture texture,
		const glm::vec3 & displacement, const GLuint & _prog = program);


	static void initialize();

	~GBasicTextureLightObject();

};

