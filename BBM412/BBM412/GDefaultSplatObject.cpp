#include "GDefaultSplatObject.h"

#include <sstream>
#include <iostream>


GLuint GDefaultSplatObject::program;
GLuint GDefaultSplatObject::uniform_index[3];

GDefaultSplatObject::GDefaultSplatObject(const string & obj, const GLuint & _prog) :
	GDefaultSplatObject(obj, obj, glm::mat4(1), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GDefaultSplatObject::GDefaultSplatObject(const string & sub, const std::string & obj, const GLuint & _prog) :
	GDefaultSplatObject(sub, obj, glm::mat4(1), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GDefaultSplatObject::GDefaultSplatObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog) :
	GDefaultSplatObject(obj, obj, glm::translate(dis), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GDefaultSplatObject::GDefaultSplatObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular,
	const GLfloat & shininess, const GLuint & _prog) : GDefaultSplatObject(obj, obj, glm::translate(dis), _m_specular, shininess)
{
}

GDefaultSplatObject::GDefaultSplatObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog) :
	GDefaultSplatObject(sub, obj, glm::translate(dis), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GDefaultSplatObject::GDefaultSplatObject(const string & sub, const string & obj, const glm::mat4 & _model, const GLuint & _prog) :
	GDefaultSplatObject(sub, obj, _model, glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{

}

GDefaultSplatObject::GDefaultSplatObject(const string & sub, const string & obj, const glm::mat4 & _model, const glm::vec3 & _m_specular,
	const GLfloat & _shininess, const GLuint & _prog) : GObject(_model), m_specular(_m_specular), shininess(_shininess), current_program(_prog)
{
#ifdef _DEBUG
	throwError("GDefaultObject()[enter]:" + obj + "\n");
#endif
	stringstream ss;
	vector<string> texts;

	ss << "objects\\" << obj << "\\" << obj << ".obj";
	string object{ ss.str() };	ss.str("");

	ss << "texture\\" << obj << "\\text1.dds";
	texts.push_back(ss.str());	ss.str("");

	ss << "texture\\" << obj << "\\text2.dds";
	texts.push_back(ss.str());	ss.str("");

	ss << "texture\\" << obj << "\\text3.dds";
	texts.push_back(ss.str());	ss.str("");

	ss << "texture\\" << obj << "\\text4.dds";
	texts.push_back(ss.str());	ss.str("");

	ss << "texture\\" << obj << "\\splat.dds";
	texts.push_back(ss.str());	ss.str("");


	initTexture(texts);

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

#ifdef _DEBUG
	throwError("GDefaultObject()[exit]:" + obj + "\n");
#endif
}

GDefaultSplatObject::GDefaultSplatObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const GLuint & _prog) :
	GDefaultSplatObject(vbo, vao, _points_size, glm::mat4{ 1.0f }, glm::vec3{ 1.0f }, 32.0f, _prog)
{
}

GDefaultSplatObject::GDefaultSplatObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const glm::mat4 & _model, const GLuint & _prog) :
	GDefaultSplatObject(vbo, vao, _points_size, glm::mat4{ 1.0f }, glm::vec3{ 1.0f }, 32.0f, _prog)
{

}

GDefaultSplatObject::GDefaultSplatObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,
	const glm::mat4 & _model, const glm::vec3 & _m_specular, const GLfloat & _shininess, const GLuint & _prog) :
	VBO(vbo), VAO(vao), points_size(_points_size), GObject(_model), m_specular(_m_specular), shininess(_shininess), current_program(_prog)
{

}





void GDefaultSplatObject::render()
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
	case RenderMode::REFLECTION_CALC:
		reflectionCalculationRender();
		break;

	}
#ifdef _DEBUG
	throwError("GDefaultSplatObject::render():\n" + to_string(typeid(this).hash_code()));
#endif
}

void GDefaultSplatObject::toggleRender(const glm::mat4 & model)
{
	switch (renderMode) {

	case RenderMode::DEFAULT:
		defaultToggleRender(model);
		break;
	case RenderMode::BASIC_TEXTURE:
		basicTextureToggleRender(model);
		break;
	case RenderMode::WIREFRAME:
		wireframeToggleRender(model);
		break;
	case RenderMode::SHADOW_CALC:
		shadowCalculationToggleRender(model);
		break;

	}

#ifdef _DEBUG
	throwError("GDefaultSplatObject::toggleRender():" + to_string(typeid(this).hash_code()) + "\n" );
#endif // _DEBUG
}



inline void GDefaultSplatObject::initTexture(const vector<string>& texts)
{
	auto size = texts.size();
	for (unsigned int i = 0; i < size; i++)
	{

		Texture texture = createTexture(texts[i]);
		textures.push_back(texture);
		defaultTextures.push_back(texture);

		glBindTexture(texture.target, texture.tex_name);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(texture.target, 0);

	}

#ifdef _DEBUG	
	throwError("GDefaultSplatObject::initTexture():\n");
#endif
}

inline void GDefaultSplatObject::defaultRender()
{
	glUseProgram(program);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	glUniform1i(120, GObject::nums_uniform_buffers[0]);
	glUniform1i(121, GObject::nums_uniform_buffers[1]);
	glUniform1i(122, GObject::nums_uniform_buffers[2]);


	glUniform3fv(110, 1, &m_specular[0]);
	glUniform1f(111, shininess);


	int text = 0;
	for (auto texture : textures)
	{
		glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit + text);
		glBindTexture(texture.target, texture.tex_name);
		glUniform1i(100 + text, nextAvaibleTextureUnit + text);
		text++;
	}


	glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(sunViewProj*GModel));
	glUniform1i(130, 0);
	glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(spotViewProj*GModel));
	glUniform1i(140, 1);

	glm::mat4 mvp;
	glm::mat4 mv;
	//glm::mat3 normal_mv;


	mvp = GObject::camera->getViewProjMatrix()*GModel;
	mv = GObject::camera->getViewMatrix()*GModel;


	//normal_mv = glm::mat3(glm::inverseTranspose(mv));

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(mv));
	//glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(normal_mv));


	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultSplatObject::basicTextureRender()
{
	glUseProgram(only_texture_program);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	int text = 0;
	for (auto texture : textures)
	{
		glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit + text);
		glBindTexture(texture.target, texture.tex_name);
		glUniform1i(100 + text, nextAvaibleTextureUnit + text);
		text++;
	}


	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultSplatObject::wireframeRender()
{
	glUseProgram(GProgram[(int)RenderMode::WIREFRAME]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(1, 1, &wireframeColor[0]);
	glDrawArrays(GL_LINES, 0, points_size);

}

inline void GDefaultSplatObject::shadowCalculationRender()
{

	glUseProgram(GProgram[SHADOW_CALC]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;

	mvp = lightViewProj*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);

#ifdef _DEBUG
	throwError("GDefaultSplatObject::shadowCalculationRender:" + to_string(typeid(this).hash_code()) + "\n");
#endif // _DEBUG

}

inline void GDefaultSplatObject::reflectionCalculationRender()
{
	glUseProgram(only_texture_program);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = reflectionMatrix*GModel;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	int text = 0;
	for (auto texture : textures)
	{
		glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit + text);
		glBindTexture(texture.target, texture.tex_name);
		glUniform1i(100 + text, nextAvaibleTextureUnit + text);
		text++;
	}


	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultSplatObject::defaultToggleRender(const glm::mat4 & model)
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



	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultSplatObject::basicTextureToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::BASIC_TEXTURE]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));


	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultSplatObject::wireframeToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::WIREFRAME]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(1, 1, &wireframeColor[0]);
	glDrawArrays(GL_LINES, 0, points_size);

}

inline void GDefaultSplatObject::shadowCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[SHADOW_CALC]);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = lightViewProj*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultSplatObject::reflectionCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(only_texture_program);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model*reflectionMatrix;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	int text = 0;
	for (auto texture : textures)
	{
		glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit + text);
		glBindTexture(texture.target, texture.tex_name);
		glUniform1i(100 + text, nextAvaibleTextureUnit + text);
		text++;
	}


	glDrawArrays(GL_TRIANGLES, 0, points_size);
}


#ifdef _DEBUG
void GDefaultSplatObject::debugRender()
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


	glUniform3fv(110, 1, &m_specular[0]);
	glUniform1f(111, shininess);


	int text = 0;
	for (auto texture : textures)
	{
		glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit + text);
		glBindTexture(texture.target, texture.tex_name);
		glUniform1i(100 + text, nextAvaibleTextureUnit + text);
		text++;
	}


	glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(lightViewProj));
	glUniform1i(130, 0);

	glm::mat4 mvp;
	glm::mat4 mv;
	glm::mat3 normal_mv;


	mvp = debug_viewProj*GModel;
	mv = debug_view*GModel;


	//normal_mv = glm::mat3(glm::inverseTranspose(mv));

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(mv));
	//glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(normal_mv));


	glDrawArrays(GL_TRIANGLES, 0, points_size);


}
#endif

GLuint GDefaultSplatObject::only_texture_program;

void GDefaultSplatObject::initialize()
{

	program = createProgram("shaders\\gsplat\\defaultsplat.vert.glsl", "shaders\\gsplat\\defaultsplat.frag.glsl");
	only_texture_program = createProgram("shaders\\gsplat\\onlytexture.vert.glsl", "shaders\\gsplat\\onlytexture.frag.glsl");

	uniform_index[0] = glGetUniformBlockIndex(program, "DirectLight");
	uniform_index[1] = glGetUniformBlockIndex(program, "PointLight");
	uniform_index[2] = glGetUniformBlockIndex(program, "SpotLight");

	glUniformBlockBinding(program, uniform_index[0], 0);
	glUniformBlockBinding(program, uniform_index[1], 1);
	glUniformBlockBinding(program, uniform_index[2], 2);

#ifdef _DEBUG
	throwError("GDefaultSplatObject::initialize():\n");
#endif // _DEBUG



}

GDefaultSplatObject::~GDefaultSplatObject()
{
}