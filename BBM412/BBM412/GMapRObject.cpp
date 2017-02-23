#include "GMapRObject.h"

#include <sstream>
#include <iostream>


GLuint GMapRObject::program;

void GMapRObject::setReflectionTexture(Texture tex)
{
	texture = tex;
}

GMapRObject::GMapRObject(const string & obj, const GLuint & _prog) :
	GMapRObject(obj, obj, glm::mat4(1), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GMapRObject::GMapRObject(const string & sub, const std::string & obj, const GLuint & _prog) :
	GMapRObject(sub, obj, glm::mat4(1), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GMapRObject::GMapRObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog) :
	GMapRObject(obj, obj, glm::translate(dis), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GMapRObject::GMapRObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular,
	const GLfloat & shininess, const GLuint & _prog) : GMapRObject(obj, obj, glm::translate(dis), _m_specular, shininess)
{
}

GMapRObject::GMapRObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog) :
	GMapRObject(sub, obj, glm::translate(dis), glm::vec3{ .5,.5,.5 }, 16.0f, _prog)
{
}

GMapRObject::GMapRObject(const string & sub, const string & obj, const glm::mat4 & _model, const GLuint & _prog) :
	GMapRObject(sub, obj, _model, glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{

}

GMapRObject::GMapRObject(const string & sub, const string & obj, const glm::mat4 & _model, const glm::vec3 & _m_specular,
	const GLfloat & _shininess, const GLuint & _prog) : GObject(_model), m_specular(_m_specular), shininess(_shininess), current_program(_prog)
{
	
	construct(sub, obj, vao, vbo, points_size);
#ifdef _DEBUG
	throwError("GMapRObject()[exit]:" + obj + "\n");
#endif
}

GMapRObject::GMapRObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const GLuint & _prog) :
	GMapRObject(vbo, vao, _points_size, glm::vec3{ 0.0f }, glm::vec3{ 1.0f }, 32.0f, _prog)
{
}

GMapRObject::GMapRObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const glm::vec3 & displacement, const GLuint & _prog) :
	GMapRObject(vbo, vao, _points_size, glm::vec3{ 0.0f }, glm::vec3{ 1.0f }, 32.0f, _prog)
{

}

GMapRObject::GMapRObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,
	const glm::vec3 & displacement, const glm::vec3 & _m_specular, const GLfloat & _shininess, const GLuint & _prog) :
	vbo(vbo), vao(vao), points_size(_points_size), GObject(glm::translate(displacement)), m_specular(_m_specular), shininess(_shininess), current_program(_prog)
{

}

void GMapRObject::construct(const string & sub, const string & obj, GLuint & vao, GLuint & vbo, unsigned int & points_size)
{
	vector<glm::vec3> points;
	vector<glm::vec3> normals;

	stringstream ss;

	ss << "objects\\" << sub << "\\" << obj << ".obj";
	string object{ ss.str() };	ss.str("");


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	readObjectFileVN(object, points, normals);
	points_size = points.size();

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size() + sizeof(glm::vec3)*normals.size(), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3)*points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size(), sizeof(glm::vec3)*normals.size(), &normals[0]);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(glm::vec3)*points.size()));


	glBindVertexArray(0);

#ifdef _DEBUG
	throwError("GMapRObject()[exit]:" + obj + "\n");
#endif
}



void GMapRObject::render()
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
	throwError("GMapRObject::render():" + d_getObjectIdentity() + "\n");
#endif // _DEBUG

}

void GMapRObject::toggleRender(const glm::mat4 & model_matrix)
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
	throwError("GMapRObject::toggleRender():\n" + to_string(typeid(this).hash_code()));
#endif // _DEBUG
}


inline void GMapRObject::defaultRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

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
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);


	glDrawArrays(GL_TRIANGLES, 0, points_size);


}

inline void GMapRObject::basicTextureRender()
{
	glUseProgram(GProgram[TEXTURE]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));


	glActiveTexture(GL_TEXTURE0 + nextAvaibleTextureUnit);
	glBindTexture(texture.target, texture.tex_name);
	glUniform1i(100, nextAvaibleTextureUnit);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
#ifdef _DEBUG
	throwError("GMapRObject::basicTextureRender():" + d_getObjectIdentity() + "\n");
#endif // _DEBUG
}

inline void GMapRObject::wireframeRender()
{
	glUseProgram(GProgram[WIREFRAME]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(1, 1, &wireframeColor[0]);
	glDrawArrays(GL_LINES, 0, points_size);
#ifdef _DEBUG
	throwError("GMapRObject::wireframeRender():" + d_getObjectIdentity() + "\n");
#endif // _DEBUG

}

inline void GMapRObject::shadowCalculationRender()
{

	glUseProgram(GProgram[SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;

	mvp = lightViewProj*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
		to_string( glGetUniformLocation(GProgram[SHADOW_CALC],"mvp") ) );

	glDrawArrays(GL_TRIANGLES, 0, points_size);

#ifdef _DEBUG
	throwError("GMapRObject::shadowCalculationRender():" + d_getObjectIdentity() + "\n");
#endif // _DEBUG

}

inline void GMapRObject::reflectionCalculationRender()
{
	glUseProgram(GProgram[TEXTURE]);
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

inline void GMapRObject::defaultToggleRender(const glm::mat4 & model)
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
	glUniform1i(100, nextAvaibleTextureUnit);

	glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(lightViewProj));
	glUniform1i(130, GL_TEXTURE0);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GMapRObject::basicTextureToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[TEXTURE]);
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

inline void GMapRObject::wireframeToggleRender(const glm::mat4 & model)
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

inline void GMapRObject::shadowCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = lightViewProj*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GMapRObject::reflectionCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[TEXTURE]);
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




void GMapRObject::initialize()
{

	program = createProgram("shaders\\greflection\\onlyreflection.vert.glsl", "shaders\\greflection\\onlyreflection.frag.glsl");
	GLuint uniform_index[3];
	uniform_index[0] = glGetUniformBlockIndex(program, "DirectLight");
	uniform_index[1] = glGetUniformBlockIndex(program, "PointLight");
	uniform_index[2] = glGetUniformBlockIndex(program, "SpotLight");

	glUniformBlockBinding(program, uniform_index[0], 0);
	glUniformBlockBinding(program, uniform_index[1], 1);
	glUniformBlockBinding(program, uniform_index[2], 2);



}

GMapRObject::~GMapRObject()
{
}
#ifdef _DEBUG

void GMapRObject::debugRender()
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
	throwError("GMapRObject::debugRender():" + to_string(typeid(this).hash_code()));
#endif // _DEBUG

}
glm::vec3 GMapRObject::getReflectionNormal()
{
	return glm::vec3{ 0,1,0 };
}
#endif // _DEBUG

