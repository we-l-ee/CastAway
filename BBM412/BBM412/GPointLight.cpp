#include "GPointLight.h"



GPointLight::GPointLight(const PointLightData & _pointlight, const GLuint & _vao, const GLuint & _vbo,
	const GLuint & _points_size, const glm::vec4 & _color, const glm::vec3 & dis, const GLuint & _prog):
	GObject(glm::translate(dis)), PointLight(_pointlight, dis),GBasicLightObject(_vao, _vbo,_points_size,_color,dis, _prog)
{

#ifdef _DEBUG
	GObject::throwError("World::GPointLight():\n");
#endif

}

GPointLight::GPointLight(const std::string & obj, const GLuint & _prog ) : 
	GPointLight(obj, obj,  glm::vec3(.0f), _prog)
{
}

GPointLight::GPointLight(const std::string & sub, const std::string & obj, const GLuint & _prog) : 
	GPointLight(sub, obj, glm::vec3(.0f), _prog)
{
}

GPointLight::GPointLight(const std::string & obj ,const glm::vec3 & dis, const GLuint & _prog ) : 
	GPointLight(obj, obj, dis, _prog)
{
}

GPointLight::GPointLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, const GLuint & _prog ) : 
	GObject(glm::translate(dis)), PointLight(sub, obj ,dis), GBasicLightObject(sub, obj, glm::vec4(pointlight.color, glm::vec1(1)),dis), Moveable(dis)
{
#ifdef _DEBUG
	GObject::throwError("World::GPointLight():\n");
#endif
}

GPointLight::GPointLight(const std::string & obj, const GLuint & _vao, const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog ) :
	GPointLight(obj, obj, glm::vec3(.0f),_vao, _vbo, _points_size, _prog)
{
}

GPointLight::GPointLight(const std::string & sub, const std::string & obj, const GLuint & _vao, const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog) :
	GPointLight(sub, obj, glm::vec3(.0f),_vao, _vbo, _points_size, _prog)
{
}

GPointLight::GPointLight(const std::string & obj, const glm::vec3 & dis, const GLuint & _vao, const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog ) :
	GPointLight(obj, obj, dis,_vao, _vbo, _points_size, _prog)
{
}

GPointLight::GPointLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, const GLuint & _vao, const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog ) :
	GObject(glm::translate(dis)), PointLight(sub, obj, dis), GBasicLightObject(_vao,_vbo, _points_size, glm::vec4(pointlight.color, glm::vec1(1)), _prog),Moveable(dis)
{
#ifdef _DEBUG
	GObject::throwError("World::GPointLight():\n");
#endif
}


void GPointLight::translate(glm::vec3 displacement)
{
	Moveable::translate(displacement);
	GModel = calculateModelMatrix();
}

void GPointLight::translate(float x, float y, float z)
{
	Moveable::translate(x, y, z);
	GModel = calculateModelMatrix();
}

void GPointLight::translateOn(float x, float y, float z, const Directions & _dir)
{
	cout << "TransalteOn:" <<x<<","<<y<<","<<z<< endl;
	Moveable::translateOn(x, y, z, _dir);
	GModel = calculateModelMatrix();
}

void GPointLight::rotate(float x, float y, float z)
{
	Moveable::rotate(x, y, z);
	GModel = calculateModelMatrix();
}

void GPointLight::rotateAround(float x, float y, float z, const Directions & dir)
{
	Moveable::rotateAround(x, y, z, dir);
	GModel = calculateModelMatrix();
}


GPointLight::~GPointLight()
{
}
