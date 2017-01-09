#pragma once
#include "GDefaultObject.h"

#include "Moveable.h"
#include "Camera.h"
#include "DirecLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "glm\ext.hpp"

class GDynamicDefaultObject:
	public GDefaultObject, public virtual Moveable
{

protected:

	bool toggle{ false };
	inline void toggleDefaultRender();

public:

	GDynamicDefaultObject(const string & obj, const GLuint & _prog = GDefaultObject::program);
	GDynamicDefaultObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog = GDefaultObject::program);
	GDynamicDefaultObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular, const GLfloat & shininess,
		const GLuint & _prog = GDefaultObject::program);

	GDynamicDefaultObject(const string & sub, const string & obj, const GLuint & _prog = GDefaultObject::program);
	GDynamicDefaultObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog = GDefaultObject::program);
	GDynamicDefaultObject(const string & sub, const string & obj, const glm::vec3 & dis,
		const glm::vec3 & _m_specular, const GLfloat & shininess, const GLuint & _prog = GDefaultObject::program);

	GDynamicDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const GLuint & _prog = GDefaultObject::program);
	GDynamicDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const glm::mat4 & _model, const GLuint & _prog = GDefaultObject::program);
	GDynamicDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,
		const glm::mat4 & _model, const glm::vec3 & _m_specular, const GLfloat & shininess, const GLuint & _prog = GDefaultObject::program);

	bool isToggleMode();
	virtual void toggleToggleMode();

	virtual void render() override;

	// Inherited via Moveable
	virtual void translate(glm::vec3 displacement) override;

	virtual void translate(float x, float y, float z) override;
	virtual void translateOn(float x, float y, float z, const Directions & _dir) override;

	virtual void rotate(float x, float y, float z) override;
	virtual void rotateAround(float x, float y, float z, const Directions & dir) override;

	~GDynamicDefaultObject();
};

