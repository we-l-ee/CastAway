#include "GDefaultObject.h"

#include <sstream>
#include <iostream>


GLuint GDefaultObject::program;
GLuint GDefaultObject::uniform_index[3];

void GDefaultObject::setTexture(Texture tex)
{
	texture = tex;
}

void GDefaultObject::setDefaultTexture()
{
	texture = defaultTexture;
}

GDefaultObject::GDefaultObject(const string & obj, const GLuint & _prog): 
	GDefaultObject(obj, obj, glm::mat4(1), glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{
}

GDefaultObject::GDefaultObject(const string & sub, const std::string & obj, const GLuint & _prog) : 
	GDefaultObject(sub, obj, glm::mat4(1), glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{
}

GDefaultObject::GDefaultObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog ) : 
	GDefaultObject(obj, obj, glm::translate(dis), glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{
}

GDefaultObject::GDefaultObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular, 
	const GLfloat & shininess, const GLuint & _prog): GDefaultObject(obj,obj, glm::translate(dis), _m_specular, shininess)
{
}

GDefaultObject::GDefaultObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog ) : 
	GDefaultObject(sub, obj, glm::translate(dis), glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{
}

GDefaultObject::GDefaultObject(const string & sub, const string & obj, const glm::mat4 & _model, const GLuint & _prog ):
	GDefaultObject(sub, obj, _model, glm::vec3{.5,.5,.5}, 32.0f,_prog)
{

}

GDefaultObject::GDefaultObject(const string & sub, const string & obj, const glm::mat4 & _model, const glm::vec3 & _m_specular,
	const GLfloat & _shininess, const GLuint & _prog):	model(_model), m_specular(_m_specular), shininess(_shininess),current_program(_prog)
{
#ifdef _DEBUG
	throwError("GDefaultObject()[enter]:"+obj+"\n");
#endif
	stringstream ss;

	ss << "objects\\" << sub << "\\" << obj << ".obj";
	string object{ ss.str() };	ss.str("");

	ss << "texture\\" << sub << "\\" << obj << ".dds";
	string text{ ss.str() };

#if (DEBUG_LVL>=1)
	std::cout << object << endl << text << endl;
#endif

	vector<glm::vec3> points;
	vector<glm::vec2> text_cords;
	vector<glm::vec3> normals;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	readObjectFile(object, points, text_cords, normals);
	points_size = points.size();

	initTexture(text);

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

#ifdef _DEBUG
	throwError("GDefaultObject()[exit]:" + obj + "\n");
#endif
}

GDefaultObject::GDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,const GLuint & _prog ):
	GDefaultObject(vbo,vao,_points_size,glm::mat4{1.0f}, glm::vec3{ 1.0f }, 32.0f, _prog)
{
}

GDefaultObject::GDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const glm::mat4 & _model, const GLuint & _prog ):
	GDefaultObject(vbo,vao,_points_size, glm::mat4{1.0f}, glm::vec3{1.0f}, 32.0f,_prog)
{

}

GDefaultObject::GDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, 
	const glm::mat4 & _model, const glm::vec3 & _m_specular, const GLfloat & _shininess, const GLuint & _prog ):
	VBO(vbo), VAO(vao), points_size(_points_size), model(_model), m_specular(_m_specular), shininess(_shininess),current_program(_prog)
{

}



void GDefaultObject::render()
{
	switch (renderMode) {

	case RenderMode::DEFAULT:
		defaultRender();
		break;
	case RenderMode::BASIC_TEXTURE:
		basicTextureRender();
		break;
	case RenderMode::WIREFRAME:
		wireframeRender();
		break;
	case RenderMode::SHADOW_CALC:
		shadowCalculationRender();
		break;
	
	}

#ifdef _DEBUG
	throwError("GDefaultObject::render():\n");
#endif // _DEBUG

}

#ifdef _DEBUG
void GDefaultObject::toggleRender(const glm::mat4 & model_matrix)
{
	switch (renderMode) {

	case RenderMode::DEFAULT:
		defaultToggleRender(model_matrix);
		break;
	case RenderMode::BASIC_TEXTURE:
		basicTextureToggleRender(model_matrix);
		break;
	case RenderMode::WIREFRAME:
		wireframeToggleRender(model_matrix);
		break;
	case RenderMode::SHADOW_CALC:
		shadowCalculationToggleRender(model_matrix);
		break;

	}

#ifdef _DEBUG
	throwError("GDefaultObject::toggleRender():\n");
#endif // _DEBUG
}
#endif // _DEBUG


inline void GDefaultObject::defaultRender()
{
	glUseProgram(current_program);
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

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, GL_TEXTURE0 + nextAvaibleTextureUnit);
	
	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultObject::basicTextureRender()
{
	glUseProgram( GProgram[(int) RenderMode::BASIC_TEXTURE ] );
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));


	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, GL_TEXTURE0 + nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultObject::wireframeRender()
{
	glUseProgram(GProgram[(int)RenderMode::WIREFRAME]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(2, 1, &wireframeColor[0]);

	glDrawArrays(GL_LINE_LOOP, 0, points_size);
}

inline void GDefaultObject::shadowCalculationRender()
{
	glUseProgram(GProgram[(int)RenderMode::SHADOW_CALC]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultObject::defaultToggleRender(const glm::mat4 & model)
{
	glUseProgram(current_program);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glUniform1i(120, GObject::nums_uniform_buffers[0]);
	glUniform1i(121, GObject::nums_uniform_buffers[1]);
	glUniform1i(122, GObject::nums_uniform_buffers[2]);


	glUniform3fv(101, 1, &m_specular[0]);
	glUniform1f(102, shininess);


	glm::mat4 mvp;
	glm::mat4 mv;
	glm::mat3 normal_mv;


	mvp = GObject::camera->getProjMatrix()*model;
	mv = model;


	normal_mv = glm::mat3(glm::inverseTranspose(mv));

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(normal_mv));

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, GL_TEXTURE0 + nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultObject::basicTextureToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::BASIC_TEXTURE]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));


	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, GL_TEXTURE0 + nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultObject::wireframeToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::WIREFRAME]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(2, 1, &wireframeColor[0]);

	glDrawArrays(GL_LINE_LOOP, 0, points_size);
}

inline void GDefaultObject::shadowCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::SHADOW_CALC]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}


void GDefaultObject::initTexture(string path)
{

	texture = createTexture(path);
	defaultTexture = texture;

	glBindTexture(texture.target, texture.tex_name);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(texture.target, 0);

#ifdef _DEBUG	
	throwError("GStaticObject::initTexture():\n");
#endif

}

void GDefaultObject::initialize()
{

	program = createProgram("shaders\\gdefaultobject\\default.vert.glsl", "shaders\\gdefaultobject\\default.frag.glsl");

	uniform_index[0] = glGetUniformBlockIndex(program, "DirectLight");
	uniform_index[1] = glGetUniformBlockIndex(program, "PointLight");
	uniform_index[2] = glGetUniformBlockIndex(program, "SpotLight");

	glUniformBlockBinding(program, uniform_index[0], 0);
	glUniformBlockBinding(program, uniform_index[1], 1);
	glUniformBlockBinding(program, uniform_index[2], 2);

	throwError("GStaticObject::render():\n");


}

GDefaultObject::~GDefaultObject()
{
}
#ifdef _DEBUG

void GDefaultObject::debugRender()
{
	glUseProgram(current_program);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glUniform1i(120, GObject::nums_uniform_buffers[0]);
	glUniform1i(121, GObject::nums_uniform_buffers[1]);
	glUniform1i(122, GObject::nums_uniform_buffers[2]);


	glUniform3fv(101, 1, &m_specular[0]);
	glUniform1f(102, shininess);


	glm::mat4 mvp;
	glm::mat4 mv;
	glm::mat3 normal_mv;


	mvp = debug_viewProj*model;
	mv = debug_view*model;


	normal_mv = glm::mat3(glm::inverseTranspose(mv));

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(normal_mv));

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, GL_TEXTURE0 + nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}
#endif // _DEBUG

