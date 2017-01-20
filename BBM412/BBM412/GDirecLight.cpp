#include "GDirecLight.h"






GDirecLight::GDirecLight(const std::string & obj, const glm::vec3 & dis, const GLuint & _prog ) : 
	GDirecLight(obj, obj, dis , _prog)
{
}
GDirecLight::GDirecLight(const std::string & obj, const glm::vec4 & _color, const GLuint & _prog ) :
	GDirecLight(obj, obj,glm::vec3(.0f),_color, _prog)
{
}

GDirecLight::GDirecLight(const std::string & sub, const std::string & obj, const GLuint & _prog) : 
	GDirecLight(sub, obj, glm::vec3(.0f), _prog)
{
}
GDirecLight::GDirecLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, const GLuint & _prog) : 
	GObject(glm::translate(dis)), DirecLight(sub, obj, dis), GBasicLightObject(sub, obj, glm::vec4(direclight.color, glm::vec1(1)), dis, _prog)
{
}

GDirecLight::GDirecLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, const glm::vec4 & _color, const GLuint & _prog):
	GObject(glm::translate(dis)), DirecLight(sub, obj, dis), GBasicLightObject(sub, obj, glm::vec4(_color), dis, _prog)
{
}


GDirecLight::GDirecLight(const std::string & obj, const GLuint & _vao, const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog ):
	GDirecLight(obj , obj , glm::vec3(.0f) ,_vao, _vbo,_points_size,_prog)
{
}

GDirecLight::GDirecLight(const std::string & sub, const std::string & obj, const GLuint & _vao, const GLuint & _vbo,
	const GLuint & _points_size, const GLuint & _prog ):
	GDirecLight(sub, obj, glm::vec3(.0f), _vao, _vbo, _points_size, _prog)
{
}

GDirecLight::GDirecLight(const std::string & obj, const glm::vec3 & dis, const GLuint & _vao, const GLuint & _vbo,
	const GLuint & _points_size, const GLuint & _prog ):
	GDirecLight(obj, obj, dis, _vao, _vbo, _points_size, _prog)
{
}

GDirecLight::GDirecLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, const GLuint & _vao,
	const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog ):
	GObject(glm::translate(dis)), DirecLight(sub, obj, dis), GBasicLightObject(_vao, _vbo, _points_size, glm::vec4(direclight.color, glm::vec1(1)), _prog), Moveable(dis)
{
}


void GDirecLight::translate(glm::vec3 displacement)
{
	Moveable::translate(displacement);
	GModel = calculateModelMatrix();
}

void GDirecLight::translate(float x, float y, float z)
{
	Moveable::translate(x, y, z);
	GModel = calculateModelMatrix();
}

void GDirecLight::translateOn(float x, float y, float z, const Directions & _dir)
{
	Moveable::translateOn(x, y, z, _dir);
	GModel = calculateModelMatrix();
}

void GDirecLight::rotate(float x, float y, float z)
{
	Moveable::rotate(x, y, z);
	GModel = calculateModelMatrix();
}

void GDirecLight::rotateAround(float x, float y, float z, const Directions & dir)
{
	Moveable::rotateAround(x, y, z, dir);
	GModel = calculateModelMatrix();
}



GDirecLight::~GDirecLight()
{
}
