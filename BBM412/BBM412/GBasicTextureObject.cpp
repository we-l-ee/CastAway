#include "GBasicTextureObject.h"

#include <sstream>
#include <iostream>


GLuint GBasicTextureObject::program;


void GBasicTextureObject::setTexture(Texture tex)
{
	texture = tex;
}

void GBasicTextureObject::setDefaultTexture()
{
	texture = defaultTexture;
}

GBasicTextureObject::GBasicTextureObject(const string & obj, const GLuint & _prog) : GBasicTextureObject(obj,obj, glm::vec3{ .0f }, _prog)
{
}

GBasicTextureObject::GBasicTextureObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog ) : GBasicTextureObject(obj,obj,dis, _prog)
{
}

GBasicTextureObject::GBasicTextureObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog ) : GBasicTextureObject(obj, obj, glm::translate(dis), _prog)
{
}

GBasicTextureObject::GBasicTextureObject(const string & sub,const string & obj, const glm::mat4 & _model, const GLuint & _prog ) :
	model(_model) , current_program(_prog)
{
#ifdef _DEBUG
	throwError("GBasicTextureObject()[enter]:\n");
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

	readObjectFile(object, points, text_cords);
	points_size = points.size();

	initTexture(text);

	glGenBuffers(1, &VBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size() +
		sizeof(glm::vec2)*text_cords.size() + sizeof(glm::vec3)*normals.size(), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3)*points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size(), sizeof(glm::vec2)*text_cords.size(), &text_cords[0]);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(glm::vec3)*points.size()));

	glBindVertexArray(0);

#ifdef _DEBUG
	throwError("GBasicTextureObject()[exit]:\n");
#endif
}


GBasicTextureObject::GBasicTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const GLuint & _prog ) :
	GBasicTextureObject(vbo, vao, _points_size, glm::mat4{ 1.0f },_prog)
{
}

GBasicTextureObject::GBasicTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const glm::mat4 & _model, const GLuint & _prog ) :
	VBO(vbo) , VAO(vao) , points_size(_points_size) , model(_model) , current_program(_prog)
{

}


void GBasicTextureObject::render()
{
	switch (renderMode) {

	case RenderMode::DEFAULT:
	case RenderMode::BASIC_TEXTURE:
		defaultRender();
		break;
	case RenderMode::WIREFRAME:
		wireframeRender();
		break;
	case RenderMode::SHADOW_CALC:
		shadowCalculationRender();
		break;

	}

#ifdef _DEBUG
	throwError("GBasicTextureObject::render():\n");
#endif // _DEBUG

}

void GBasicTextureObject::toggleRender(const glm::mat4 & model_matrix)
{
	switch (renderMode) {

	case RenderMode::DEFAULT:
	case RenderMode::BASIC_TEXTURE:
		defaultToggleRender(model_matrix);
		break;
	case RenderMode::WIREFRAME:
		wireframeToggleRender(model_matrix);
		break;
	case RenderMode::SHADOW_CALC:
		shadowCalculationToggleRender(model_matrix);
		break;

	}

#ifdef _DEBUG
	throwError("GBasicTextureObject::toggleRender():\n");
#endif // _DEBUG
}

inline void GBasicTextureObject::defaultRender()
{
	glUseProgram(current_program);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glActiveTexture(GL_TEXTURE0+ nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, GL_TEXTURE0 + nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GBasicTextureObject::wireframeRender()
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

inline void GBasicTextureObject::shadowCalculationRender()
{
	glUseProgram(GProgram[(int)RenderMode::SHADOW_CALC]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}


inline void GBasicTextureObject::defaultToggleRender(const glm::mat4 & model)
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

inline void GBasicTextureObject::wireframeToggleRender(const glm::mat4 & model)
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

inline void GBasicTextureObject::shadowCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::SHADOW_CALC]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}


void GBasicTextureObject::initTexture(string path)
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


GBasicTextureObject::~GBasicTextureObject()
{
}

#ifdef _DEBUG
void GBasicTextureObject::debugRender()
{
	glUseProgram(current_program);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;

	mvp = debug_viewProj*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, GL_TEXTURE0 + nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}
#endif
