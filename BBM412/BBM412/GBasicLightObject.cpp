#include <sstream>
#include "GBasicLightObject.h"

GLuint GBasicLightObject::program;


GBasicLightObject::GBasicLightObject(const string & obj, const glm::vec4 & _color, const GLuint & _prog) :
	GBasicLightObject(obj, obj, _color, glm::vec3(0), _prog)
{
}

GBasicLightObject::GBasicLightObject(const string & obj, const glm::vec4 & _color, const glm::vec3 & displacement, const GLuint & _prog) :
	GBasicLightObject(obj, obj, _color, displacement, _prog)
{
}


GBasicLightObject::GBasicLightObject(const string & sub, const string & obj, const glm::vec4 & _color, const GLuint & _prog) :
	GBasicLightObject(sub, obj, _color, glm::vec3(0), _prog)
{
}

GBasicLightObject::GBasicLightObject(const string & sub, const string & obj, const glm::vec4 & _color, const glm::vec3 & displacement, const GLuint & _prog) :
	GBasicObject(sub,obj,_color, displacement,_prog),GObject(glm::translate(displacement))
{

#ifdef _DEBUG
	throwError("GBasicLightObject()[exit]:\n");
#endif
}

GBasicLightObject::GBasicLightObject(const GLuint &_vao, const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color, const GLuint & _prog) :
	GBasicLightObject(_vao, _vbo, _points_size, _color, glm::vec3{ 0 }, _prog)
{

}

GBasicLightObject::GBasicLightObject(const GLuint &_vao, const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color, const glm::vec3 & displacement, const GLuint & _prog) :
	GBasicObject(_vao, _vbo, _points_size, _color, displacement, _prog), GObject(glm::translate(displacement))

{
#ifdef _DEBUG
	throwError("GBasicObject()[exit]:\n");
#endif
}

void GBasicLightObject::initialize()
{
	program = GObject::createProgram("shaders\\glight\\basic_light.vert.glsl", "shaders\\glight\\basic_light.frag.glsl");
}

GBasicLightObject::~GBasicLightObject()
{
}
