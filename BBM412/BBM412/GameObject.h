#pragma once
#include "Camera.h"

class GameObject
{
	static unsigned int duration;//Duration of the day in seconds.
	static bool day;

protected:
	glm::vec3 g_position;//center of the objects.

	static const Camera *camera;// Current Camera that views GameObjects.
	static glm::vec3 WorldUp;// Up direction of world. Negatif of this is gravity direction.
	static double step;// time past
	static double degreeOfDay;// 1 cycle (360 degree) means 1 day so on the 90 degree it will be in zenith(noon)

public:
	GameObject();
	static void setDegreeOfDay( const double & degree);
	static void initialize(const Camera * cam, const glm::vec3 & _worldUp = glm::vec3{ 0.0f,1.0f,0.0f }, const unsigned int duration = 5);

	static void setSimulationStep( const double & _step );
	static bool isDay();

	virtual void stepSimulation() {}

	glm::vec3 getPosition() const;
	~GameObject();
};

