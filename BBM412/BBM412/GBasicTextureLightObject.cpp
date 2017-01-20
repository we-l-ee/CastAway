#include "GBasicTextureLightObject.h"
GLuint GBasicTextureLightObject::program;



GBasicTextureLightObject::GBasicTextureLightObject(const string & obj, const GLuint & _prog) : GBasicTextureLightObject(obj, obj, glm::vec3{ .0f }, _prog)
{
}

GBasicTextureLightObject::GBasicTextureLightObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog) : GBasicTextureLightObject(obj, obj, dis, _prog)
{
}

GBasicTextureLightObject::GBasicTextureLightObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog) : 
	GObject(glm::translate(dis)), GBasicTextureObject(sub, obj, dis, _prog)
{
}




GBasicTextureLightObject::GBasicTextureLightObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const Texture texture, const GLuint & _prog) :
	GBasicTextureLightObject(vbo, vao, _points_size, texture, glm::vec3 {0.0f }, _prog)
{
}

GBasicTextureLightObject::GBasicTextureLightObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const Texture texture, 
	const glm::vec3 & displacement, const GLuint & _prog) :
	GObject(glm::translate(displacement)), GBasicTextureObject(vbo,vao,_points_size, texture, displacement,_prog)
{

}

void GBasicTextureLightObject::initialize()
{
	program = createProgram("shaders\\glight\\basictexture_light.vert.glsl", "shaders\\glight\\basictexture_light.frag.glsl");

}

GBasicTextureLightObject::~GBasicTextureLightObject()
{
}
