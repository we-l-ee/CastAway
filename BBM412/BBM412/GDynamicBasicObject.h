#pragma once
#include "GBasicObject.h"
class GDynamicBasicObject :
	public GBasicObject, public virtual Moveable
{

public:
	GDynamicBasicObject(const string & obj, const glm::vec4 & _color,
		const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GDynamicBasicObject(const string & obj, const glm::vec4 & _color, const glm::vec3 & _displace,
		const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);

	GDynamicBasicObject(const string & sub, const string & obj, const glm::vec4 & _color,
		const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GDynamicBasicObject(const string & sub, const string & obj, const glm::vec4 & _color,
		const glm::vec3 & _displace, const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);

	GDynamicBasicObject(const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color,
		const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);
	GDynamicBasicObject(const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color,
		const glm::vec3 & displace, GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::WIREFRAME]);


	// Inherited via Moveable
	virtual void translate(glm::vec3 displacement) override;

	virtual void translate(float x, float y, float z) override;
	virtual void translateOn(float x, float y, float z, const Directions & _dir) override;

	virtual void rotate(float x, float y, float z) override;
	virtual void rotateAround(float x, float y, float z, const Directions & dir) override;

	~GDynamicBasicObject();

};

