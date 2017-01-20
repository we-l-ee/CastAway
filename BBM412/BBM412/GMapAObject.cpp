#include "GMapAObject.h"

GLuint GMapAObject::program;
GMapAObject::GMapAObject(const string & obj, const GLuint & _prog) :
	GMapAObject(obj, obj, glm::mat4(1), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GMapAObject::GMapAObject(const string & sub, const std::string & obj, const GLuint & _prog) :
	GMapAObject(sub, obj, glm::mat4(1), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GMapAObject::GMapAObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog) :
	GMapAObject(obj, obj, glm::translate(dis), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GMapAObject::GMapAObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular,
	const GLfloat & shininess, const GLuint & _prog) : GMapAObject(obj, obj, glm::translate(dis), _m_specular, shininess)
{
}

GMapAObject::GMapAObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog) :
	GMapAObject(sub, obj, glm::translate(dis), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GMapAObject::GMapAObject(const string & sub, const string & obj, const glm::mat4 & _model, const GLuint & _prog) :
	GMapAObject(sub, obj, _model, glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{

}

GMapAObject::GMapAObject(const string & sub, const string & obj, const glm::mat4 & _model, const glm::vec3 & _m_specular,
	const GLfloat & _shininess, const GLuint & _prog) : GObject(_model), GDefaultObject(sub,obj,_model,_m_specular,_shininess,_prog)
{

}

GMapAObject::GMapAObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const GLuint & _prog) :
	GMapAObject(vbo, vao, _points_size, glm::vec3{ 0.0f }, glm::vec3{ 0.5f }, 32.0f, _prog)
{
}

GMapAObject::GMapAObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const glm::vec3 & displacement, 
	const GLuint & _prog) :
	GMapAObject(vbo, vao, _points_size, displacement, glm::vec3{ 0.5f }, 32.0f, _prog)
{

}

GMapAObject::GMapAObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,
	const glm::vec3 & displacement, const glm::vec3 & _m_specular, const GLfloat & _shininess, const GLuint & _prog) :
	GObject(glm::translate(displacement)), GDefaultObject(vbo, vao, _points_size, displacement, _m_specular, _shininess, _prog)
{

}

void GMapAObject::initialize()
{
	program = createProgram("shaders\\gdefaultobject\\mapa.vert.glsl", "shaders\\gdefaultobject\\mapa.frag.glsl");
	GLuint uniform_index[3];
	uniform_index[0] = glGetUniformBlockIndex(program, "DirectLight");
	uniform_index[1] = glGetUniformBlockIndex(program, "PointLight");
	uniform_index[2] = glGetUniformBlockIndex(program, "SpotLight");

	glUniformBlockBinding(program, uniform_index[0], 0);
	glUniformBlockBinding(program, uniform_index[1], 1);
	glUniformBlockBinding(program, uniform_index[2], 2);
}

GMapAObject::~GMapAObject()
{
}
