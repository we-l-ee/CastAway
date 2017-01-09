#include "Sun.h"



Sun::Sun() :GDirecLight("sun")
{
}

void Sun::calculateLightening()
{

	if (degreeOfDay < 180 && !isActive())
		toggleActive();
	else if (degreeOfDay >= 180 && isActive())
		toggleActive();
	glm::vec3 cameraPosition = camera->getPosition();

	model = glm::translate(
		glm::vec3(cos(glm::radians(degreeOfDay)) * 1000, sin(glm::radians(degreeOfDay)) * 1000, 0.0) + cameraPosition )
		* glm::scale(glm::vec3(10.0f) );
	Moveable::position = glm::vec3(cos(glm::radians(degreeOfDay)) + cameraPosition.x, sin(glm::radians(degreeOfDay)) + cameraPosition.y, cameraPosition.z);

	calculateDirecLight(cameraPosition);
}


Sun::~Sun()
{
}
