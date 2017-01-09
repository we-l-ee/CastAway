#include "GDefaultSplatObject.h"

#include <sstream>
#include <iostream>


GLuint GDefaultSplatObject::program;
GLuint GDefaultSplatObject::uniform_index[3];


GDefaultSplatObject::GDefaultSplatObject(const string & obj) : GDefaultSplatObject(obj, glm::mat4{ 1.0f })
{
}

GDefaultSplatObject::GDefaultSplatObject(const string & obj, const glm::vec3 & dis) : GDefaultSplatObject(obj, glm::translate(dis))
{
}

GDefaultSplatObject::GDefaultSplatObject(const string & obj, const glm::mat4 & _model) : GDefaultSplatObject(obj, _model, glm::vec3{ .5,.5,.5 }, 32.0f)
{

}

GDefaultSplatObject::GDefaultSplatObject(const string & obj, const glm::mat4 & _model, const glm::vec3 & _m_specular, const GLfloat & _shininess) :
	model(_model), m_specular(_m_specular), shininess(_shininess)
{
	stringstream ss;

	ss << "objects\\" << obj << "\\" << obj << ".obj";
	string object{ ss.str() };	ss.str("");

	ss << "objects\\" << obj << "\\text1.dds";
	string text1{ ss.str() };	ss.str("");

	ss << "objects\\" << obj << "\\text2.dds";
	string text2{ ss.str() };	ss.str("");

	ss << "objects\\" << obj << "\\text3.dds";
	string text3{ ss.str() };	ss.str("");

	ss << "objects\\" << obj << "\\text4.dds";
	string text4{ ss.str() };	ss.str("");

	ss << "objects\\" << obj << "\\splat.dds";
	string splat{ ss.str() };	ss.str("");


	initTexture(text1,text2,text3,text4,splat);

	vector<glm::vec3> points;
	vector<glm::vec2> text_cords;
	vector<glm::vec3> normals;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	readObjectFile(object, points, text_cords, normals);
	points_size = points.size();



	glGenBuffers(1, &VBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size() +
		sizeof(glm::vec2)*text_cords.size() + sizeof(glm::vec3)*normals.size(), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3)*points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size(), sizeof(glm::vec2)*text_cords.size(), &text_cords[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size() + sizeof(glm::vec2)*text_cords.size(),
		sizeof(glm::vec3)*normals.size(), &normals[0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(glm::vec3)*points.size()));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(glm::vec3)*points.size() + sizeof(glm::vec2)*text_cords.size()));

	glBindVertexArray(0);

	throwError("GDefaultObject():\n");
}

GDefaultSplatObject::GDefaultSplatObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size) :
	GDefaultSplatObject(vbo, vao, _points_size, glm::mat4{ 1.0f })
{
}

GDefaultSplatObject::GDefaultSplatObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const glm::mat4 & _model) :
	GDefaultSplatObject(vbo, vao, _points_size, glm::mat4{ 1.0f }, glm::vec3{ 1.0f }, 32.0f)
{

}

GDefaultSplatObject::GDefaultSplatObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,
	const glm::mat4 & _model, const glm::vec3 & _m_specular, const GLfloat & _shininess) :
	VBO(vbo), VAO(vao), points_size(_points_size), model(_model), m_specular(_m_specular), shininess(_shininess)
{

}



void GDefaultSplatObject::render()
{
	switch (renderMode) {
	case RenderMode::DEFAULT:
		defaultRender();
		break;

	}
	throwError("GDynamicObject::render():\n");

}

inline void GDefaultSplatObject::defaultRender()
{
	glUseProgram(program);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffers[0]);
	//glBindBufferBase(GL_UNIFORM_BUFFER, 1, uniform_buffers[1]);
	//glBindBufferBase(GL_UNIFORM_BUFFER, 2, uniform_buffers[2]);

	glUniform1i(120, GObject::nums_uniform_buffers[0]);
	glUniform1i(121, GObject::nums_uniform_buffers[1]);
	glUniform1i(122, GObject::nums_uniform_buffers[2]);


	glUniform3fv(101, 1, &m_specular[0]);
	glUniform1f(102, shininess);


	glm::mat4 mvp;
	glm::mat4 mv;
	glm::mat3 normal_mv;


	mvp = GObject::camera->getViewProjMatrix()*model;
	mv = GObject::camera->getViewMatrix()*model;


	normal_mv = glm::mat3(glm::inverseTranspose(mv));

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(normal_mv));

	glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(GObject::camera->getViewMatrix()));


	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultSplatObject::initTexture(string text1, string text2, string text3, string text4, string splat)
{

	Texture1 = createTexture(text1);
	defaultTexture1 = Texture1;

	glBindTexture(Texture1.target, Texture1.tex_name);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(Texture1.target, 0);

#ifdef _DEBUG	
	throwError("GDefaultSplatObject::initTexture():\n");
#endif
}


void GDefaultSplatObject::initialize()
{

	program = createProgram("shaders\\gdefaultobject\\defaultsplat.vert.glsl", "shaders\\gdefaultobject\\defaultsplat.frag.glsl");

	uniform_index[0] = glGetUniformBlockIndex(program, "DirectLight");
	uniform_index[1] = glGetUniformBlockIndex(program, "PointLight");
	uniform_index[2] = glGetUniformBlockIndex(program, "SpotLight");

	glUniformBlockBinding(program, uniform_index[0], 0);
	glUniformBlockBinding(program, uniform_index[1], 1);
	glUniformBlockBinding(program, uniform_index[2], 2);

	throwError("GStaticObject::render():\n");


}

GDefaultSplatObject::~GDefaultSplatObject()
{
}