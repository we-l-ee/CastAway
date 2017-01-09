#pragma once
#include "GObject.h"
class GBasicObject:
	public GObject
{

protected:
	GLuint vao, vbo;

	GLuint points_size;
	
	glm::mat4 model;
	glm::vec4 color;
	
	GLuint current_program;

protected:

	inline void defaultRender();
	inline void wireframeRender();
	inline void shadowCalculationRender();

	inline void defaultToggleRender(const glm::mat4 & model);
	inline void wireframeToggleRender(const glm::mat4 & model);
	inline void shadowCalculationToggleRender(const glm::mat4 & model);

public:
	GBasicObject(const string & obj, const glm::vec4 & _color, 
		const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GBasicObject(const string & sub, const string & obj, const glm::vec4 & _color, 
		const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GBasicObject(const string & obj, const glm::vec4 & _color, const glm::vec3 & _displace, 

		const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GBasicObject(const string & sub, const string & obj, const glm::vec4 & _color, 
		const glm::vec3 & _displace, const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GBasicObject(const string & sub, const string & obj, const glm::vec4 & _color, 
		const glm::mat4 & _model , const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);

	GBasicObject(const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color, 
		const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GBasicObject(const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color,
		const glm::vec3 & displace, GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GBasicObject(const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color, 
		const glm::mat4 & _model, const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);

	virtual void render() override;
	virtual void toggleRender(const glm::mat4 & model) override;

	~GBasicObject();

#ifdef _DEBUG
	virtual void debugRender() override;
#endif
};

