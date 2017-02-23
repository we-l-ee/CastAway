#include "GTiledTextureObject.h"

#include <sstream>
#include <iostream>

GLuint GTiledTextureObject::program;


void GTiledTextureObject::setTiling(const GLfloat & _tile)
{
	tile = _tile;
}

void GTiledTextureObject::setTexture(Texture tex)
{
	texture = tex;
}

void GTiledTextureObject::setDefaultTexture()
{
	texture = defaultTexture;
}

GTiledTextureObject::GTiledTextureObject(const string & obj, const GLuint & _prog) : GTiledTextureObject(obj, obj, glm::vec3{ .0f }, _prog)
{
}

GTiledTextureObject::GTiledTextureObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog) : GTiledTextureObject(obj, obj, dis, _prog)
{
}

GTiledTextureObject::GTiledTextureObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog) : GTiledTextureObject(obj, obj, glm::translate(dis), _prog)
{
}

GTiledTextureObject::GTiledTextureObject(const string & sub, const string & obj, const glm::mat4 & _model, const GLuint & _prog) :
	GObject(_model), current_program(_prog)
{
#ifdef _DEBUG
	throwError("GTiledTextureObject()[enter]:\n");
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	readObjectFile(object, points, text_cords);
	points_size = points.size();

	initTexture(text, this->texture);
	defaultTexture = texture;

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

#ifdef _DEBUG
	throwError("GTiledTextureObject()[exit]:\n");
#endif
}


GTiledTextureObject::GTiledTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const Texture & texture,
	const GLuint & _prog) :
	GTiledTextureObject(vbo, vao, _points_size, texture, glm::vec3{ 0.0f }, _prog)
{
}

GTiledTextureObject::GTiledTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,  const Texture & _texture,
	const glm::vec3 & displacement, const GLuint & _prog) :
	vbo(vbo), vao(vao), points_size(_points_size),texture(_texture),defaultTexture(_texture),
	GObject(glm::translate(displacement)), current_program(_prog)
{

}

void GTiledTextureObject::construct(const string & sub, const string & obj, GLuint & vbo, GLuint & vao, unsigned int & points_size, Texture & texture)
{
	stringstream ss;

	ss << "objects\\" << sub << "\\" << obj << ".obj";
	string object{ ss.str() };	ss.str("");
	ss << "texture\\" << sub << "\\" << obj << ".dds";
	string text{ ss.str() };

	initTexture(text, texture);

	vector<glm::vec3> points;
	vector<glm::vec2> text_cords;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	readObjectFile(object, points, text_cords);
	points_size = points.size();

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


void GTiledTextureObject::render()
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
	throwError("GTiledTextureObject::render():" + to_string(typeid(this).hash_code()) + "\n");
#endif // _DEBUG

}

void GTiledTextureObject::toggleRender(const glm::mat4 & model_matrix)
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
	throwError("GTiledTextureObject::toggleRender():" + to_string(typeid(this).hash_code()) + "\n");
#endif // _DEBUG
}

void GTiledTextureObject::intialize()
{
	program = createProgram("shaders\\gtiled\\tiledtexture.vert.glsl", "shaders\\gtiled\\tiledtexture.frag.glsl");
}

inline void GTiledTextureObject::defaultRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform1f(110, tile);


	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GTiledTextureObject::wireframeRender()
{
	glUseProgram(GProgram[WIREFRAME]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(1, 1, &wireframeColor[0]);
	glDrawArrays(GL_LINES, 0, points_size);

}

inline void GTiledTextureObject::shadowCalculationRender()
{
	glUseProgram(GProgram[SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = lightViewProj*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GTiledTextureObject::reflectionCalculationRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel*reflectionMatrix;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform1f(110, tile);

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}


inline void GTiledTextureObject::defaultToggleRender(const glm::mat4 & model)
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	//glUniform1f(110, tile);


	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GTiledTextureObject::wireframeToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(1, 1, &wireframeColor[0]);
	glDrawArrays(GL_LINES, 0, points_size);

}

inline void GTiledTextureObject::shadowCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = lightViewProj*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}


void GTiledTextureObject::initTexture(const string & path, Texture & texture)
{

	texture = createTexture(path);

	glBindTexture(texture.target, texture.tex_name);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(texture.target, 0);

#ifdef _DEBUG	
	throwError("GTiledTextureObject::initTexture():\n");
#endif

}




GTiledTextureObject::~GTiledTextureObject()
{
}

#ifdef _DEBUG
void GTiledTextureObject::debugRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;

	mvp = debug_viewProj*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);

	throwError("GTiledTextureOBject::debugRender():" + to_string(typeid(this).hash_code()) + "\n");
}
#endif
