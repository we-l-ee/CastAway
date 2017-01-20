#include "DefaultDynamicObject.h"
#include <sstream>

DefaultDynamicObject::DefaultDynamicObject(const string & obj):DefaultDynamicObject(obj, glm::vec3{0.0f})
{
}

DefaultDynamicObject::DefaultDynamicObject(const string & obj, const glm::vec3 & dis):DefaultDynamicObject(obj,dis,glm::vec3{.5,.5,.5}, 32)
{
}

DefaultDynamicObject::DefaultDynamicObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular, const GLfloat & shininess):
	GObject(glm::translate(dis)), GDynamicDefaultObject(obj, dis,_m_specular, shininess)
{
	stringstream ss;
	ss << "objects\\" << obj << "\\" << obj << ".bullet";

	pObject::rigidBody = addRigidBody(ss.str(), dis);

}



void DefaultDynamicObject::translate(glm::vec3 displacement)
{
	Moveable::translate(displacement);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));
}

void DefaultDynamicObject::translate(float x, float y, float z)
{
	Moveable::translate(x, y, z);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));

}

void DefaultDynamicObject::translateOn(float x, float y, float z, const Directions & _dir)
{
	Moveable::translateOn(x, y, z, _dir);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));
}

void DefaultDynamicObject::rotate(float x, float y, float z)
{
	Moveable::rotate(x, y, z);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));

}

void DefaultDynamicObject::rotateAround(float x, float y, float z, const Directions & dir)
{
	Moveable::rotateAround(x, y, z, dir);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));

}




DefaultDynamicObject::~DefaultDynamicObject()
{
}
