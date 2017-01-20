#include "GBasicObject.h"
#include <sstream>


inline void GBasicObject::defaultRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glUniform4fv(1, 1, &this->color[0]);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GBasicObject::wireframeRender()
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

inline void GBasicObject::shadowCalculationRender()
{
	glUseProgram(GProgram[(int)RenderMode::SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = lightViewProj*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GBasicObject::reflectionCalculationRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*GModel*reflectionMatrix;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glUniform4fv(1, 1, &this->color[0]);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}


inline void GBasicObject::defaultToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::BASIC_TEXTURE]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;

	mvp = GObject::camera->getProjMatrix()*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GBasicObject::wireframeToggleRender(const glm::mat4 & model)
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

inline void GBasicObject::shadowCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = lightViewProj*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GBasicObject::reflectionCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model*reflectionMatrix;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glUniform4fv(1, 1, &this->color[0]);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

GBasicObject::GBasicObject(const string & obj, const glm::vec4 & _color, const GLuint & _prog ):
	GBasicObject(obj, obj,_color, glm::vec3(0.f), _prog ) 
{
}

GBasicObject::GBasicObject(const string & sub, const string & obj, const glm::vec4 & _color, const GLuint & _prog ) :
	GBasicObject(sub, obj, _color, glm::vec3(0.f), _prog)
{
}

GBasicObject::GBasicObject(const string & obj, const glm::vec4 & _color, const glm::vec3 & displacement, const GLuint & _prog) :
	GBasicObject(obj, obj,_color, displacement, _prog)
{
}

GBasicObject::GBasicObject(const string & sub, const string & obj, const glm::vec4 & _color, const glm::vec3 & displacement, const GLuint & _prog ) :
	color(_color), GObject(glm::translate(displacement)), current_program(_prog)
{
	construct(sub, obj, vao, vbo, points_size);
#ifdef _DEBUG
	throwError("GBasicObject()[exit]:\n");
#endif
}


GBasicObject::GBasicObject(const GLuint & vao, const GLuint & vbo, const GLuint & _points_size, const glm::vec4 & _color, const GLuint & _prog ) :
	GBasicObject(vao,vbo,_points_size,_color,glm::vec3{0},_prog)
{

}

GBasicObject::GBasicObject(const GLuint & _vao, const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color, const glm::vec3 & displace, const GLuint & _prog):
	vao(_vao), vbo(_vbo), points_size(_points_size), color(_color), current_program(_prog), GObject(glm::translate(displace))
{


}


void GBasicObject::construct(const string & sub, const string & obj, GLuint & vao, GLuint & vbo, unsigned int & points_size)
{
	stringstream ss;
	ss << "objects\\" << sub << "\\" << obj << ".obj";
	vector<glm::vec3> points;

	readObjectFile(ss.str(), points);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	points_size = points.size();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size(), &points[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
#ifdef _DEBUG
	GObject::throwError("GBasicObject::construct():\n");
#endif // _DEBUG
}

void GBasicObject::render()
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
	throwError("GBasicObject::render():\n");
#endif // _DEBUG

}


void GBasicObject::toggleRender(const glm::mat4 & model_matrix)
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
	throwError("GBasicObject::toggleRender():\n");
#endif // _DEBUG

}

GBasicObject::~GBasicObject()
{
}

#ifdef _DEBUG

void GBasicObject::debugRender()
{

	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp = debug_viewProj*GModel;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(1, 1, &color[0]);

	glDrawArrays(GL_TRIANGLES, 0, points_size);

	throwError("GBasicObject::debugRender():");

}
#endif