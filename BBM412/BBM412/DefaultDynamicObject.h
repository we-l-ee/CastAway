#pragma once
#include "GObject.h"
#include "pDynamicObject.h"
#include "Moveable.h"
#include "Camera.h"

#include "PointLight.h"
#include "DirecLight.h"
#include "SpotLight.h"

#include "GDynamicDefaultObject.h"

class DefaultDynamicObject:
	public GDynamicDefaultObject, public pDynamicObject
{


protected:



public:

	DefaultDynamicObject(const string & obj);
	DefaultDynamicObject(const string & obj, const glm::vec3 & dis);
	DefaultDynamicObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular, const GLfloat & shininess);

	
	~DefaultDynamicObject(); 

	// Inherited via Moveable
	virtual void translate(glm::vec3 displacement) override;

	virtual void translate(float x, float y, float z) override;
	virtual void translateOn(float x, float y, float z, const Directions & _dir) override;

	virtual void rotate(float x, float y, float z) override;
	virtual void rotateAround(float x, float y, float z, const Directions & dir) override;


};

