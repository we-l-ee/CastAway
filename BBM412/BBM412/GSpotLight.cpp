#include "GSpotLight.h"



GSpotLight::GSpotLight(const std::string & obj, const GLuint & _prog ) : GSpotLight(obj, obj, glm::vec3(.0f), _prog)
{
}

GSpotLight::GSpotLight(const std::string & sub, const std::string & obj, const GLuint & _prog ) : GSpotLight(sub, obj, glm::vec3(.0f), _prog)
{
}

GSpotLight::GSpotLight(const std::string & obj, const glm::vec3 & dis, const GLuint & _prog ) : GSpotLight(obj, obj, dis, _prog)
{
}

GSpotLight::GSpotLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, const GLuint & _prog ) :
	SpotLight(sub, obj, dis), GBasicLightObject(sub, obj, glm::vec4(spotlight.color, glm::vec1(1)), dis)
{
}

GSpotLight::GSpotLight(const std::string & obj, const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog ) :
	GSpotLight(obj, obj, glm::vec3(.0f), _vbo, _points_size, _prog)
{
}

GSpotLight::GSpotLight(const std::string & sub, const std::string & obj, const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog) :
	GSpotLight(sub, obj, glm::vec3(.0f), _vbo, _points_size, _prog)
{
}

GSpotLight::GSpotLight(const std::string & obj, const glm::vec3 & dis, const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog ) :
	GSpotLight(obj, obj, dis, _vbo, _points_size, _prog)
{
}

GSpotLight::GSpotLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog) :
	SpotLight(sub, obj, dis), GBasicLightObject(_vbo, _points_size, glm::vec4(spotlight.color, glm::vec1(1)), _prog)
{
}


void GSpotLight::translate(glm::vec3 displacement)
{
	Moveable::translate(displacement);
	model = calculateModelMatrix();
}

void GSpotLight::translate(float x, float y, float z)
{
	Moveable::translate(x, y, z);
	model = calculateModelMatrix();
}

void GSpotLight::translateOn(float x, float y, float z, const Directions & _dir)
{
	Moveable::translateOn(x, y, z, _dir);
	model = calculateModelMatrix();
}

void GSpotLight::rotate(float x, float y, float z)
{
	Moveable::rotate(x, y, z);
	model = calculateModelMatrix();
}

void GSpotLight::rotateAround(float x, float y, float z, const Directions & dir)
{
	Moveable::rotateAround(x, y, z, dir);
	model = calculateModelMatrix();
}



GSpotLight::~GSpotLight()
{
}
