#include "DefaultDynamicObject.h"
#include <sstream>

DefaultDynamicObject::DefaultDynamicObject(const string & obj):DefaultDynamicObject(obj, glm::vec3{0.0f})
{
}

DefaultDynamicObject::DefaultDynamicObject(const string & obj, const glm::vec3 & dis):DefaultDynamicObject(obj,dis,glm::vec3{.5,.5,.5}, 32)
{
}

DefaultDynamicObject::DefaultDynamicObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular, const GLfloat & shininess):
	GDynamicDefaultObject(obj, dis,_m_specular, shininess)
{
	stringstream ss;
	ss << "objects\\" << obj << "\\" << obj << ".bullet";

	addRigidBody(ss.str(), dis);

}

DefaultDynamicObject::~DefaultDynamicObject()
{
}
