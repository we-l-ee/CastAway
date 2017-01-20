#include "DefaultStaticObject.h"





DefaultStaticObject::DefaultStaticObject(const string & obj):DefaultStaticObject(obj,glm::vec3{0,0,0})
{
}

DefaultStaticObject::DefaultStaticObject(const string & obj, const glm::vec3 & dis):
	GObject(glm::translate(dis)), GDefaultObject(obj,dis)
{
	stringstream ss;
	ss << "objects\\" << obj << "\\" << obj << ".bullet";

	pObject::rigidBody = addRigidBody( ss.str(), dis );

}

DefaultStaticObject::DefaultStaticObject(const string & obj, const GLuint & vao, const GLuint & vbo, const unsigned int & points_size, const glm::vec3 & dis):
	GObject(glm::translate(dis)),GDefaultObject(vbo,vao,points_size, dis)
{
	stringstream ss;
	ss << "objects\\" << obj << "\\" << obj << ".bullet";

	pObject::rigidBody = addRigidBody(ss.str(), dis);
}

DefaultStaticObject::~DefaultStaticObject()
{
}
