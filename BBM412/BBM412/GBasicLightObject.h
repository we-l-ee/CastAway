#pragma once
#include "GBasicObject.h"
class GBasicLightObject:
	public GBasicObject
{


protected:
	static GLuint program;

public:
	GBasicLightObject(const string & obj, const glm::vec4 & _color,
		const GLuint & _prog = program);
	GBasicLightObject(const string & obj, const glm::vec4 & _color, const glm::vec3 & _displace,
		const GLuint & _prog = program);

	GBasicLightObject(const string & sub, const string & obj, const glm::vec4 & _color,
		const GLuint & _prog = program);
	GBasicLightObject(const string & sub, const string & obj, const glm::vec4 & _color,
		const glm::vec3 & _displace, const GLuint & _prog = program);


	GBasicLightObject(const GLuint &_vao, const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color,
		const GLuint & _prog = program);
	GBasicLightObject(const GLuint &_vao, const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color,
		const glm::vec3 & displacement, const GLuint & _prog = program);



	static void initialize();

	~GBasicLightObject();

};

