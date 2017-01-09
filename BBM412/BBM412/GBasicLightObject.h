#pragma once
#include "GObject.h"
class GBasicLightObject:
	public GObject
{

protected:
	static GLuint program;

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
	GBasicLightObject(const string & obj, const glm::vec4 & _color,
		const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GBasicLightObject(const string & sub, const string & obj, const glm::vec4 & _color,
		const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GBasicLightObject(const string & obj, const glm::vec4 & _color, const glm::vec3 & _displace,
		const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GBasicLightObject(const string & sub, const string & obj, const glm::vec4 & _color,
		const glm::vec3 & _displace, const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GBasicLightObject(const string & sub, const string & obj, const glm::vec4 & _color,
		const glm::mat4 & _model, const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);

	GBasicLightObject(const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color,
		const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GBasicLightObject(const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color,
		const glm::mat4 & _model, const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);

	virtual void render() override;
	virtual void toggleRender(const glm::mat4 & model) override;


	static void initialize();

	~GBasicLightObject();

#ifdef _DEBUG
	virtual void debugRender() override;
#endif
};

