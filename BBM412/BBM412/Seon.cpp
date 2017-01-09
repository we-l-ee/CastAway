#include "Seon.h"
#include <sstream>

GLuint Seon::VAO, Seon::VBO, Seon::points_size;
PointLightData Seon::POINTLIGHT(glm::vec3{ 0,0,0 }, glm::vec3{ 1,1,1 });

Seon::Seon(const glm::vec3 & dis, const glm::vec3 &_color):
	GPointLight(POINTLIGHT,VBO, points_size, glm::vec4( _color,glm::vec1(1.0) ), dis )
{
	pointlight.color = _color;
#ifdef _DEBUG
	throwError("Seon()[exit]:\n");
#endif
}

void Seon::initialize()
{

	vector<glm::vec3> points;
	GObject::readObjectFile("objects\\seon\\seon.obj", points);

	POINTLIGHT = readLightFile("objects\\seon\\seon.light");

	points_size = points.size();

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points_size, &points[0], GL_STATIC_DRAW);

}

Seon::~Seon()
{
}

