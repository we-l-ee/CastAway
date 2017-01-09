#include "GDynamicBasicObject.h"
#include <sstream>


GDynamicBasicObject::GDynamicBasicObject(const string & obj, const glm::vec4 & _color, const GLuint & _prog) :
	GBasicObject(obj, obj, _color, glm::mat4{}, _prog), Moveable()
{
}

GDynamicBasicObject::GDynamicBasicObject(const string & obj, const glm::vec4 & _color, const glm::vec3 & displace, const GLuint & _prog) :
	GBasicObject(obj, obj, _color,glm::translate(displace), _prog), Moveable(displace)
{
}


GDynamicBasicObject::GDynamicBasicObject(const string & sub, const string & obj, const glm::vec4 & _color, const GLuint & _prog) :
	GBasicObject(sub, obj, _color, glm::mat4{}, _prog), Moveable()
{
}

GDynamicBasicObject::GDynamicBasicObject(const string & sub, const string & obj, const glm::vec4 & _color, const glm::vec3 & displace, const GLuint & _prog) :
	GBasicObject(sub, obj, _color, glm::translate(displace), _prog),Moveable(displace)
{

}


GDynamicBasicObject::GDynamicBasicObject(const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color, const GLuint & _prog):
	GBasicObject(_vbo, _points_size, _color, glm::mat4{}, _prog), Moveable()
{
}

GDynamicBasicObject::GDynamicBasicObject(const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color, const glm::vec3 & displace, GLuint & _prog):
	GBasicObject(_vbo, _points_size, _color, displace, _prog), Moveable(displace)
{
}

GDynamicBasicObject::~GDynamicBasicObject()
{
}



void GDynamicBasicObject::translate(glm::vec3 displacement)
{
	Moveable::translate(displacement);
	model = calculateModelMatrix();
}

void GDynamicBasicObject::translate(float x, float y, float z)
{
	Moveable::translate(x, y, z);
	model = calculateModelMatrix();
}

void GDynamicBasicObject::translateOn(float x, float y, float z, const Directions & _dir)
{
	Moveable::translateOn(x, y, z, _dir);
	model = calculateModelMatrix();
}

void GDynamicBasicObject::rotate(float x, float y, float z)
{
	Moveable::rotate(x, y, z);
	model = calculateModelMatrix();
}

void GDynamicBasicObject::rotateAround(float x, float y, float z, const Directions & dir)
{
	Moveable::rotateAround(x, y, z, dir);
	model = calculateModelMatrix();
}