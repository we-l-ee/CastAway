#include "DefaultStaticObject.h"





DefaultStaticObject::DefaultStaticObject(const string & obj):DefaultStaticObject(obj,glm::vec3{0,0,0})
{
}

DefaultStaticObject::DefaultStaticObject(const string & obj, const glm::vec3 & dis):
	GDefaultObject(obj,dis)
{
	stringstream ss;
	ss << "objects\\" << obj << "\\" << obj << ".bullet";

	addRigidBody( ss.str(), dis );

}

DefaultStaticObject::~DefaultStaticObject()
{
}
