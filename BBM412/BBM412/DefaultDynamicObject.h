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


};

