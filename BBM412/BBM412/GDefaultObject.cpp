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
	GDefaultObject(obj, obj, glm::mat4(1), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GDefaultObject::GDefaultObject(const string & sub, const std::string & obj, const GLuint & _prog) : 
	GDefaultObject(sub, obj, glm::mat4(1), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GDefaultObject::GDefaultObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog ) : 
	GDefaultObject(obj, obj, glm::translate(dis), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GDefaultObject::GDefaultObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular, 
	const GLfloat & shininess, const GLuint & _prog): GDefaultObject(obj,obj, glm::translate(dis), _m_specular, shininess)
{
}

GDefaultObject::GDefaultObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog ) : 
	GDefaultObject(sub, obj, glm::translate(dis), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GDefaultObject::GDefaultObject(const string & sub, const string & obj, const glm::mat4 & _model, const GLuint & _prog ):
	GDefaultObject(sub, obj, _model, glm::vec3{.5,.5,.5}, 32.0f,_prog)
{

}

GDefaultObject::GDefaultObject(const string & sub, const string & obj, const glm::mat4 & _model, const glm::vec3 & _m_specular,
	const GLfloat & _shininess, const GLuint & _prog):	GObject(_model), m_specular(_m_specular), shininess(_shininess),current_program(_prog)
{
	vector<glm::vec3> points;
	vector<glm::vec2> text_cords;
	vector<glm::vec3> normals;

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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	readObjectFile(object, points, text_cords, normals);
	points_size = points.size();

	texture = initTexture(text);
	defaultTexture = texture;

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

GDefaultObject::GDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,const GLuint & _prog ):
	GDefaultObject(vbo,vao,_points_size,glm::vec3{0.0f}, glm::vec3{ 1.0f }, 32.0f, _prog)
{
}

GDefaultObject::GDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const glm::vec3 & displacement, const GLuint & _prog ):
	GDefaultObject(vbo,vao,_points_size, glm::vec3{0.0f}, glm::vec3{1.0f}, 32.0f,_prog)
{

}

GDefaultObject::GDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, 
	const glm::vec3 & displacement, const glm::vec3 & _m_specular, const GLfloat & _shininess, const GLuint & _prog ):
	vbo(vbo), vao(vao), points_size(_points_size), GObject(glm::translate(displacement)), m_specular(_m_specular), shininess(_shininess),current_program(_prog)
{

}

void GDefaultObject::construct(const string & sub, const string & obj, GLuint & vao, GLuint & vbo, unsigned int & points_size, Texture & texture)
{
	vector<glm::vec3> points;
	vector<glm::vec2> text_cords;
	vector<glm::vec3> normals;

#ifdef _DEBUG
	throwError("GDefaultObject()[enter]:" + obj + "\n");
#endif
	stringstream ss;

	ss << "objects\\" << sub << "\\" << obj << ".obj";
	string object{ ss.str() };	ss.str("");

	ss << "texture\\" << sub << "\\" << obj << ".dds";
	string text{ ss.str() };

#if (DEBUG_LVL>=1)
	std::cout << object << endl << text << endl;
#endif

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	readObjectFile(object, points, text_cords, normals);
	points_size = points.size();

	texture = initTexture(text);

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
	case RenderMode::REFLECTION_CALC:
		reflectionCalculationRender();
		break;
	}

#ifdef _DEBUG
	throwError("GDefaultObject::render():\n");
#endif // _DEBUG

}

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


inline void GDefaultObject::defaultRender()
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
	glUniform1i(130, 0 );
	glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(spotViewProj*GModel));
	glUniform1i(140, 1);


	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);


	glDrawArrays(GL_TRIANGLES, 0, points_size);


}

inline void GDefaultObject::basicTextureRender()
{
	glUseProgram( GProgram[(int) RenderMode::BASIC_TEXTURE ] );
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));


	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100,  nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultObject::wireframeRender()
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

inline void GDefaultObject::shadowCalculationRender()
{

	glUseProgram(GProgram[(int)RenderMode::SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;

	mvp = lightViewProj*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);

#ifdef _DEBUG
	throwError("shadowCalculationRender::render():\n");
#endif // _DEBUG

}

inline void GDefaultObject::reflectionCalculationRender()
{
	glUseProgram(GProgram[(int)RenderMode::BASIC_TEXTURE]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel*reflectionMatrix;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));


	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultObject::defaultToggleRender(const glm::mat4 & model)
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

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(mv));

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100,  nextAvaibleTextureUnit);

	glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(sunViewProj*GModel));
	glUniform1i(130, 0);
	glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(spotViewProj*GModel));
	glUniform1i(140, 1);


	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultObject::basicTextureToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::BASIC_TEXTURE]);
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

inline void GDefaultObject::wireframeToggleRender(const glm::mat4 & model)
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

inline void GDefaultObject::shadowCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = lightViewProj*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GDefaultObject::reflectionCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::BASIC_TEXTURE]);
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


Texture GDefaultObject::initTexture(const string & path)
{

	Texture texture = createTexture(path);

	glBindTexture(texture.target, texture.tex_name);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(texture.target, 0);

	return texture;
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

#ifdef _DEBUG
	throwError("GDefaultObject::initialize():\n");
#endif // _DEBUG

}

GDefaultObject::~GDefaultObject()
{
}
#ifdef _DEBUG

void GDefaultObject::debugRender()
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

	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);

#ifdef _DEBUG
	throwError("GDefaultObject::debugRender():");
#endif // _DEBUG

}
#endif // _DEBUG

