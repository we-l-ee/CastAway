#include "GDynamicDefaultObject.h"

#include <sstream>
#include <iostream>



GDynamicDefaultObject::GDynamicDefaultObject(const string & obj,const GLuint & _prog ) : 
									GDynamicDefaultObject(obj , obj , glm::vec3(.0f), glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{
}

GDynamicDefaultObject::GDynamicDefaultObject(const string & sub, const string & obj, const GLuint & _prog) :
										GDynamicDefaultObject(sub, obj, glm::vec3(.0f), glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{
}

GDynamicDefaultObject::GDynamicDefaultObject(const string & obj, const glm::vec3 & dis,const GLuint & _prog ) : 
									GDynamicDefaultObject(obj, dis, glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{	
}

GDynamicDefaultObject::GDynamicDefaultObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular, const GLfloat & shininess, const GLuint & _prog):
	GDynamicDefaultObject(obj, obj, dis, _m_specular, shininess, _prog)

{
}

GDynamicDefaultObject::GDynamicDefaultObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog ) : 
									GDynamicDefaultObject(sub, obj, dis, glm::vec3{ .5,.5,.5 }, 32.0f, _prog)
{
}

GDynamicDefaultObject::GDynamicDefaultObject(const string & sub, const string & obj, const glm::vec3 & dis,
	const glm::vec3 & _m_specular, const GLfloat & shininess, const GLuint & _prog ) 
	: GObject(glm::translate(dis)), GDefaultObject{ sub, obj ,{},_m_specular,shininess,_prog }, Moveable{ dis }
{
#ifdef _DEBUG
	GObject::throwError("World::GDynamicDefaultObject():\n");
#endif
}



GDynamicDefaultObject::GDynamicDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const GLuint & _prog ) :
	GDynamicDefaultObject(vbo, vao, _points_size, glm::vec3{ 0.0f }, glm::vec3{ 1.0f }, 32.0f, _prog)
{
}

GDynamicDefaultObject::GDynamicDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, 
	const glm::vec3 & displacement, const GLuint & _prog ) :
	GDynamicDefaultObject(vbo, vao, _points_size, displacement, glm::vec3(0.5,0.5,0.5), 32.0f, _prog)
{
}

GDynamicDefaultObject::GDynamicDefaultObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,
	const glm::vec3 & displacement, const glm::vec3 & _m_specular, const GLfloat & _shininess, const GLuint & _prog ) :
	GObject(glm::translate(displacement)),GDefaultObject(vbo, vao, points_size, displacement, _m_specular, _shininess, _prog)
{
#ifdef _DEBUG
	GObject::throwError("World::GDynamicDefaultObject():\n");
#endif
}



void GDynamicDefaultObject::translate(glm::vec3 displacement)
{
	Moveable::translate(displacement);
	GModel = calculateModelMatrix();
}

void GDynamicDefaultObject::translate(float x, float y, float z)
{
	Moveable::translate(x, y, z);
	GModel = calculateModelMatrix();
}

void GDynamicDefaultObject::translateOn(float x, float y, float z, const Directions & _dir)
{
	Moveable::translateOn(x, y, z, _dir);
	GModel = calculateModelMatrix();
}

void GDynamicDefaultObject::rotate(float x, float y, float z)
{
	Moveable::rotate(x, y, z);
	GModel = calculateModelMatrix();
}

void GDynamicDefaultObject::rotateAround(float x, float y, float z, const Directions & dir)
{
	Moveable::rotateAround(x, y, z, dir);
	GModel = calculateModelMatrix();
}


GDynamicDefaultObject::~GDynamicDefaultObject()
{
}