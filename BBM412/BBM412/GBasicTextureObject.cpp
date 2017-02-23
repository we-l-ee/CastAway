#include "GBasicTextureObject.h"

#include <sstream>
#include <iostream>




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

GBasicTextureObject::GBasicTextureObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog ) : 
	GObject(glm::translate(dis)), current_program(_prog)
{
	construct(sub, obj, vbo, vao, points_size, texture);
	defaultTexture = texture;

#ifdef _DEBUG
	throwError("GBasicTextureObject()[exit]:\n"+ to_string(typeid(this).hash_code()));
#endif
}


GBasicTextureObject::GBasicTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const Texture texture, 
	const GLuint & _prog ) :
	GBasicTextureObject(vbo, vao, _points_size, texture, glm::vec3{ 0.0f },_prog)
{
}

GBasicTextureObject::GBasicTextureObject(const GLuint & _vbo, const GLuint & _vao, const unsigned int & _points_size, const Texture _texture, 
	const glm::vec3 & displacement, const GLuint & _prog ) :
	vbo(_vbo) , vao(_vao) , points_size(_points_size), texture(_texture), defaultTexture(_texture), GObject(glm::translate(displacement)) , current_program(_prog)
{

}

void GBasicTextureObject::construct(const string & sub, const string &obj, GLuint & vbo, GLuint & vao, unsigned int & points_size, Texture & texture)
{
	stringstream ss;

	ss << "objects\\" << sub << "\\" << obj << ".obj";
	string object{ ss.str() };	ss.str("");

	ss << "texture\\" << sub << "\\" << obj << ".dds";
	string text{ ss.str() };


	vector<glm::vec3> points;
	vector<glm::vec2> text_cords;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	readObjectFile(object, points, text_cords);
	points_size = points.size();

	texture = initTexture(text);

	glGenBuffers(1, &vbo);


	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points_size + sizeof(glm::vec2)*points_size, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3)*points_size, &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points_size, sizeof(glm::vec2)*points_size, &text_cords[0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(glm::vec3)*points_size));

	glBindVertexArray(0);
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
	case RenderMode::REFLECTION_CALC:
		reflectionCalculationRender();
		break;

	}

#ifdef _DEBUG
	throwError("GBasicTextureObject::render():\n"+ typeid(this).hash_code());
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
	throwError("GBasicTextureObject::toggleRender():\n"+ typeid(this).hash_code());
#endif // _DEBUG
}

inline void GBasicTextureObject::defaultRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100,   nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GBasicTextureObject::wireframeRender()
{
	glUseProgram(GProgram[(int)RenderMode::WIREFRAME]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(1, 1, &wireframeColor[0]);
	glDrawArrays(GL_LINES, 0, points_size);

}

inline void GBasicTextureObject::shadowCalculationRender()
{
	glUseProgram(GProgram[SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = lightViewProj*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GBasicTextureObject::reflectionCalculationRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = reflectionMatrix*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}


inline void GBasicTextureObject::defaultToggleRender(const glm::mat4 & model)
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));


	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GBasicTextureObject::wireframeToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::WIREFRAME]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(1, 1, &wireframeColor[0]);
	glDrawArrays(GL_LINES, 0, points_size);

}

inline void GBasicTextureObject::shadowCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = lightViewProj*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GBasicTextureObject::reflectionCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model*reflectionMatrix;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}


Texture GBasicTextureObject::initTexture(const string & path)
{

	Texture texture = createTexture(path);

	glBindTexture(texture.target, texture.tex_name);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(texture.target, 0);

#ifdef _DEBUG	
	throwError("GBasicTextureObject::initTexture():\n" );
#endif
	return texture;
}




GBasicTextureObject::~GBasicTextureObject()
{
}

#ifdef _DEBUG
void GBasicTextureObject::debugRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;

	mvp = debug_viewProj*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100,  nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);

	throwError("GBasicTextureObject::debugRender():"+ typeid(this).hash_code());
}
#endif
