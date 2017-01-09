#include <sstream>
#include "GBasicLightObject.h"

GLuint GBasicLightObject::program;

inline void GBasicLightObject::defaultRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glUniform4fv(2, 1, &this->color[0]);

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GBasicLightObject::wireframeRender()
{
	glUseProgram(GProgram[(int)RenderMode::WIREFRAME]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(2, 1, &wireframeColor[0]);

	glDrawArrays(GL_LINE_LOOP, 0, points_size);
}

inline void GBasicLightObject::shadowCalculationRender()
{
	glUseProgram(GProgram[(int)RenderMode::SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}


inline void GBasicLightObject::defaultToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::BASIC_TEXTURE]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;

	mvp = GObject::camera->getProjMatrix()*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}

inline void GBasicLightObject::wireframeToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::WIREFRAME]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getProjMatrix()*model;


	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4fv(2, 1, &wireframeColor[0]);

	glDrawArrays(GL_LINE_LOOP, 0, points_size);
}

inline void GBasicLightObject::shadowCalculationToggleRender(const glm::mat4 & model)
{
	glUseProgram(GProgram[(int)RenderMode::SHADOW_CALC]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp;


	mvp = GObject::camera->getViewProjMatrix()*model;

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}



GBasicLightObject::GBasicLightObject(const string & obj, const glm::vec4 & _color, const GLuint & _prog) :
	GBasicLightObject(obj, obj, _color, glm::mat4(1.f), _prog)
{
}

GBasicLightObject::GBasicLightObject(const string & sub, const string & obj, const glm::vec4 & _color, const GLuint & _prog) :
	GBasicLightObject(sub, obj, _color, glm::mat4(1.f), _prog)
{
}

GBasicLightObject::GBasicLightObject(const string & obj, const glm::vec4 & _color, const glm::vec3 & _displace, const GLuint & _prog) :
	GBasicLightObject(obj, obj, _color, glm::translate(_displace), _prog)
{
}

GBasicLightObject::GBasicLightObject(const string & sub, const string & obj, const glm::vec4 & _color, const glm::vec3 & _displace, const GLuint & _prog) :
	GBasicLightObject(sub, obj, _color, glm::translate(_displace), _prog)
{

}

GBasicLightObject::GBasicLightObject(const string & sub, const string & obj, const glm::vec4 & _color, const glm::mat4 & _model, const GLuint & _prog) :
	color(_color), model(_model), current_program(_prog)
{
#ifdef _DEBUG
	throwError("GBasicObject()[enter]:\n");
#endif

	stringstream ss;
	ss << "objects\\" << sub << "\\" << obj << ".obj";
	vector<glm::vec3> points;

	readObjectFile(ss.str(), points);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	points_size = points.size();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size(), &points[0], GL_STATIC_DRAW);

#ifdef _DEBUG
	throwError("GBasicObject()[exit]:\n");
#endif
}

GBasicLightObject::GBasicLightObject(const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color, const GLuint & _prog) :
	GBasicLightObject(_vbo, _points_size, _color, glm::mat4{ 1.0f }, _prog)
{

}

GBasicLightObject::GBasicLightObject(const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color, const glm::mat4 & _model, const GLuint & _prog) :
	vbo(_vbo), points_size(_points_size), color(_color), model(_model), current_program(_prog)

{
#ifdef _DEBUG
	throwError("GBasicObject()[enter]:\n");
#endif
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

#ifdef _DEBUG
	throwError("GBasicObject()[exit]:\n");
#endif
}

void GBasicLightObject::render()
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
	throwError("GBasicLightObject::render():\n");
#endif // _DEBUG

}

#ifdef _DEBUG
void GBasicLightObject::toggleRender(const glm::mat4 & model_matrix)
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
	throwError("GBasicLightObject::toggleRender():\n");
}
#endif // _DEBUG

void GBasicLightObject::initialize()
{
	program = createProgram("shaders\\glight\\basic_light.vert.glsl", "shaders\\glight\\basic_light.frag.glsl");
}

GBasicLightObject::~GBasicLightObject()
{
}


#ifdef _DEBUG
void GBasicLightObject::debugRender()
{
	glUseProgram(current_program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glm::mat4 mvp = debug_viewProj*model;

	glUniform4fv(2, 1, &color[0]);
	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, points_size);
}
#endif
