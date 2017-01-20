#include "StaticDefaultSplatObject.h"
#include <sstream>


StaticDefaultSplatObject::StaticDefaultSplatObject(const string & obj) :StaticDefaultSplatObject(obj, glm::vec3{ 0,0,0 })
{
}

StaticDefaultSplatObject::StaticDefaultSplatObject(const string & obj, const glm::vec3 & dis) :
	GObject(glm::translate(dis)), GDefaultSplatObject(obj, dis)
{
	stringstream ss;
	ss << "objects\\" << obj << "\\" << obj << ".bullet";

	pObject::rigidBody = addRigidBody(ss.str(), dis);
}

StaticDefaultSplatObject::~StaticDefaultSplatObject()
{
}
