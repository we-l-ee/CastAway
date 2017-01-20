#pragma once
#include "GDefaultObject.h"
class GMapAObject:
	public GDefaultObject
{
protected:
	static GLuint program;
public:

	GMapAObject(const string & obj, const GLuint & _prog = program);
	GMapAObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog = program);
	GMapAObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular,
		const GLfloat & shininess, const GLuint & _prog = program);

	GMapAObject(const string & sub, const std::string & obj, const GLuint & _prog = program);
	GMapAObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog = program);
	GMapAObject(const string & sub, const string & obj, const glm::mat4 & _model, const GLuint & _prog = program);
	GMapAObject(const string & sub, const string & obj, const glm::mat4 & _model,
		const glm::vec3 & _m_specular, const GLfloat & shininess, const GLuint & _prog = program);

	GMapAObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const GLuint & _prog = program);
	GMapAObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,
		const glm::vec3 & displacement, const GLuint & _prog = program);
	GMapAObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,
		const glm::vec3 & displacement, const glm::vec3 & _m_specular, const GLfloat & shininess, const GLuint & _prog = program);


	static void initialize();

	~GMapAObject();
};

