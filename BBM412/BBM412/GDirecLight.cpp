#include "GDirecLight.h"



GDirecLight::GDirecLight(const std::string & obj, const GLuint & _prog ) : 
	GDirecLight(obj, obj,glm::vec3(.0f), _prog)
{
}



GDirecLight::GDirecLight(const std::string & obj, const glm::vec3 & dis, const GLuint & _prog ) : 
	GDirecLight(obj, obj, dis , _prog)
{
}

GDirecLight::GDirecLight(const std::string & sub, const std::string & obj, const GLuint & _prog) : 
	GDirecLight(sub, obj, glm::vec3(.0f), _prog)
{
}
GDirecLight::GDirecLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, const GLuint & _prog) : 
	DirecLight(sub, obj, dis), GBasicLightObject(sub, obj, glm::vec4(direclight.color, glm::vec1(1)), dis, _prog)
{
}


GDirecLight::GDirecLight(const std::string & obj, const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog ):
	GDirecLight(obj , obj , glm::vec3(.0f) , _vbo,_points_size,_prog)
{
}

GDirecLight::GDirecLight(const std::string & sub, const std::string & obj, const GLuint & _vbo, 
	const GLuint & _points_size, const GLuint & _prog ) :
	GDirecLight(sub, obj, glm::vec3(.0f), _vbo, _points_size, _prog)
{
}

GDirecLight::GDirecLight(const std::string & obj, const glm::vec3 & dis, const GLuint & _vbo, 
	const GLuint & _points_size, const GLuint & _prog ):
	GDirecLight(obj, obj, dis, _vbo, _points_size, _prog)
{
}

GDirecLight::GDirecLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, 
	const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog ):
	DirecLight(sub, obj, dis), GBasicLightObject(_vbo, _points_size, glm::vec4(direclight.color, glm::vec1(1)), _prog)
{
}


void GDirecLight::translate(glm::vec3 displacement)
{
	Moveable::translate(displacement);
	model = calculateModelMatrix();
}

void GDirecLight::translate(float x, float y, float z)
{
	Moveable::translate(x, y, z);
	model = calculateModelMatrix();
}

void GDirecLight::translateOn(float x, float y, float z, const Directions & _dir)
{
	Moveable::translateOn(x, y, z, _dir);
	model = calculateModelMatrix();
}

void GDirecLight::rotate(float x, float y, float z)
{
	Moveable::rotate(x, y, z);
	model = calculateModelMatrix();
}

void GDirecLight::rotateAround(float x, float y, float z, const Directions & dir)
{
	Moveable::rotateAround(x, y, z, dir);
	model = calculateModelMatrix();
}



GDirecLight::~GDirecLight()
{
}
