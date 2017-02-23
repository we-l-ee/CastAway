#include "GDefaultMultiTextureObject.h"

#include <sstream>
#include <iostream>


GLuint GDefaultMultiTextureObject::program;

GDefaultMultiTextureObject::GDefaultMultiTextureObject(const string & obj, const GLuint & _prog) :
	GDefaultMultiTextureObject(obj, obj, glm::vec3{ 0 }, _prog)
{
}

GDefaultMultiTextureObject::GDefaultMultiTextureObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog) :
	GDefaultMultiTextureObject(obj, obj, dis, _prog)
{
}

GDefaultMultiTextureObject::GDefaultMultiTextureObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular,
	const GLfloat & shininess, const GLuint & _prog) : GDefaultMultiTextureObject(obj, obj, dis, _m_specular, shininess)
{
}

GDefaultMultiTextureObject::GDefaultMultiTextureObject(const string & sub, const string & obj, const GLuint & _prog) :
	GDefaultMultiTextureObject(sub, obj, glm::vec3{ 0 }, _prog)
{

}

GDefaultMultiTextureObject::GDefaultMultiTextureObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog) :
	GDefaultMultiTextureObject(sub, obj, dis, glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GDefaultMultiTextureObject::GDefaultMultiTextureObject(const string & sub, const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular,
	const GLfloat & _shininess, const GLuint & _prog) : GObject(glm::translate(dis)), m_specular(_m_specular), shininess(_shininess), current_program(_prog)
{
#ifdef _DEBUG
	throwError("GDefaultMultiTextureObject()[enter]:" + obj + "\n");
#endif
	stringstream ss;

	ss << "objects\\" << sub << "\\" << obj << ".obj";
	string object{ ss.str() };	ss.str("");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	vector<string> textures;
	
	//switch_points.push_back(0);

	readObjectFile(object, points, text_cords, normals,textures,switch_points);
	points_size = points.size();
	switch_points.push_back(points.size());

	this->textures = initTextures(textures);
	this->defaultTextures = this->textures;

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
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


GDefaultMultiTextureObject::GDefaultMultiTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, 
	const vector<Texture>& _textures, const vector<unsigned int>& _switch_points, const GLuint & _prog):
	GDefaultMultiTextureObject(vbo, vao, _points_size, _textures, switch_points, glm::vec3{ 0 },_prog)
{
}

GDefaultMultiTextureObject::GDefaultMultiTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,
	const vector<Texture>& _textures, const vector<unsigned int>& _switch_points, const glm::vec3 & displacement, const GLuint & _prog):
	GDefaultMultiTextureObject(vbo, vao, _points_size, _textures, _switch_points, displacement, glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GDefaultMultiTextureObject::GDefaultMultiTextureObject(const GLuint & _vbo, const GLuint & _vao, const unsigned int & _points_size, 
	const vector<Texture>& _textures, const vector<unsigned int>& _switch_points, const glm::vec3 & displacement, 
	const glm::vec3 & _m_specular, const GLfloat & _shininess, const GLuint & _prog):GObject(glm::translate(displacement)),
	vbo(_vbo), vao(_vao), points_size(_points_size), textures(_textures),defaultTextures(_textures),switch_points(_switch_points),
	m_specular(_m_specular), shininess(_shininess), current_program(_prog)
{
}


void GDefaultMultiTextureObject::render()
{
	glDisable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);

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
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);

#ifdef _DEBUG
	throwError("GDefaultMultiTextureObject::render():" + to_string(typeid(this).hash_code()) + "\n");
#endif // _DEBUG

}

void GDefaultMultiTextureObject::toggleRender(const glm::mat4 & model_matrix)
{
	glDisable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);

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
	case RenderMode::REFLECTION_CALC:
		reflectionCalculationToggleRender(model_matrix);
		break;
	}
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
#ifdef _DEBUG
	throwError("GDefaultObject::toggleRender():" + to_string(typeid(this).hash_code()) + "\n");
#endif // _DEBUG
}

void GDefaultMultiTextureObject::constructer(const string & sub, const string & obj, vector<Texture> & textures, vector<unsigned int>& switch_points, GLuint & vao, GLuint & vbo, unsigned int & points_size)
{
	vector<glm::vec3> points;
	vector<glm::vec2> text_cords;
	vector<glm::vec3> normals;
	stringstream ss;

	ss << "objects\\" << sub << "\\" << obj << ".obj";
	string object{ ss.str() };	ss.str("");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	vector<string> textures_s;

	//switch_points.push_back(0);

	readObjectFile(object, points, text_cords, normals, textures_s, switch_points);
	points_size = points.size();

	textures = initTextures(textures_s);

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
	throwError("GDefaultObject::constructer():\n");
#endif // _DEBUG
}

vector<Texture> GDefaultMultiTextureObject::initTextures(const vector<string>& texts)
{
	auto size = texts.size();
	vector<Texture> textures;

	for (unsigned int i = 0; i < size; i++)
	{

		Texture texture = createTexture(texts[i]);
		textures.push_back(texture);
		glBindTexture(texture.target, texture.tex_name);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(texture.target, 0);

	}

#ifdef _DEBUG	
	throwError("GDefaultMultiTextureObject::initTexture():\n");
#endif
	return textures;
}


inline void GDefaultMultiTextureObject::defaultRender()
{

	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glUniform1i(120, GObject::nums_uniform_buffers[0]);

	glUniform1i(121, GObject::nums_uniform_buffers[1]);

	glUniform1i(122, GObject::nums_uniform_buffers[2]);


	glUniform3fv(101, 1, &m_specular[0]);
	glUniform1f(102, shininess);


	glm::mat4 mvp;
	glm::mat4 mv;
	glm::mat3 normal_mv;


	mvp = GObject::camera->getViewProjMatrix()*GModel;
	mv = GObject::camera->getViewMatrix()*GModel;


	//normal_mv = glm::mat3(glm::transpose(glm::inverse(mv)));

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(mv));
	//glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(normal_mv));

	glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(sunViewProj*GModel));
	glUniform1i(130, 0);
	glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(spotViewProj*GModel));
	glUniform1i(140, 1);

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glUniform1i(100, nextAvaibleTextureUnit);

	int s = 0;
	while (s + 1 < switch_points.size() )
	{
			glBindTexture(textures[s].target, textures[s].tex_name);
			glDrawArrays(GL_TRIANGLES, switch_points[s], switch_points[s+1] - switch_points[s]	);
			s++;
	}

}

inline void GDefaultMultiTextureObject::basicTextureRender()
{
	glUseProgram(GProgram[(int)RenderMode::BASIC_TEXTURE]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	int s = 0;
	while (s + 1 < switch_points.size())
	{
		glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit + s);
		glBindTexture(textures[s].target, textures[s].tex_name);
		glUniform1i(100, nextAvaibleTextureUnit + s);
		glDrawArrays(GL_TRIANGLES, switch_points[s], switch_points[s + 1]);
		s++;
	}

}

inline void GDefaultMultiTextureObject::wireframeRender()
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

inline void GDefaultMultiTextureObject::shadowCalculationRender()
{

	glUseProgram(GProgram[SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;

	mvp = lightViewProj*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);

#ifdef _DEBUG
	throwError("GDefaultMultiTextureObject::shadowCalculationRender():" + d_getObjectIdentity() + "\n");
#endif // _DEBUG

}

inline void GDefaultMultiTextureObject::reflectionCalculationRender()
{
	glUseProgram(GProgram[TEXTURE]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = reflectionMatrix*GModel;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	int s = 0;
	while (s + 1 < switch_points.size())
	{
		glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit + s);
		glBindTexture(textures[s].target, textures[s].tex_name);
		glUniform1i(100, nextAvaibleTextureUnit + s);
		glDrawArrays(GL_TRIANGLES, switch_points[s], switch_points[s + 1]);
		s++;
	}
}

inline void GDefaultMultiTextureObject::defaultToggleRender(const glm::mat4 & model)
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

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

	int s = 0;
	while (s + 1 < switch_points.size())
	{
		glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit + s);
		glBindTexture(textures[s].target, textures[s].tex_name);
		glUniform1i(100, nextAvaibleTextureUnit + s);
		glDrawArrays(GL_TRIANGLES, switch_points[s], switch_points[s + 1]);
		s++;
	}

}

inline void GDefaultMultiTextureObject::basicTextureToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::BASIC_TEXTURE]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));


	int s = 0;
	while (s + 1 < switch_points.size())
	{
		glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit + s);
		glBindTexture(textures[s].target, textures[s].tex_name);
		glUniform1i(100, nextAvaibleTextureUnit + s);
		glDrawArrays(GL_TRIANGLES, switch_points[s], switch_points[s + 1]);
		s++;
	}

}

inline void GDefaultMultiTextureObject::wireframeToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[WIREFRAME]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(1, 1, &wireframeColor[0]);

	glDrawArrays(GL_LINES, 0, points_size);

}

inline void GDefaultMultiTextureObject::shadowCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = lightViewProj*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultMultiTextureObject::reflectionCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[TEXTURE]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = reflectionMatrix*GModel;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	int s = 0;
	while (s + 1 < switch_points.size())
	{
		glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit + s);
		glBindTexture(textures[s].target, textures[s].tex_name);
		glUniform1i(100, nextAvaibleTextureUnit + s);
		glDrawArrays(GL_TRIANGLES, switch_points[s], switch_points[s + 1]);
		s++;
	}
}



GDefaultMultiTextureObject::~GDefaultMultiTextureObject()
{
}
void GDefaultMultiTextureObject::initialize()
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
#ifdef _DEBUG

void GDefaultMultiTextureObject::debugRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glUniform1i(120, GObject::nums_uniform_buffers[0]);
	glUniform1i(121, GObject::nums_uniform_buffers[1]);
	glUniform1i(122, GObject::nums_uniform_buffers[2]);


	glUniform3fv(101, 1, &m_specular[0]);
	glUniform1f(102, shininess);


	glm::mat4 mvp;
	glm::mat4 mv;
	glm::mat3 normal_mv;


	mvp = debug_viewProj*GModel;
	mv = debug_view*GModel;


	//normal_mv = glm::mat3(glm::inverseTranspose(mv));

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(mv));
	//glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(normal_mv));

	int s = 0;
	while (s + 1 < switch_points.size())
	{
		glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit + s);
		glBindTexture(textures[s].target, textures[s].tex_name);
		glUniform1i(100, nextAvaibleTextureUnit + s);
		glDrawArrays(GL_TRIANGLES, switch_points[s], switch_points[s + 1]);
		s++;
	}

#ifdef _DEBUG
	throwError("GDefaultObject::debugRender():" + to_string(typeid(this).hash_code()) + "\n");
#endif // _DEBUG

}
#endif // _DEBUG

