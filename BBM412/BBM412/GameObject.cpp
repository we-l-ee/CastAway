#include "GameObject.h"

const Camera *GameObject::camera;
glm::vec3 GameObject::WorldUp;
double GameObject::step;
double GameObject::degreeOfDay;
unsigned int GameObject::duration;
bool GameObject::day;

GameObject::GameObject()
{
}

void GameObject::setDegreeOfDay(const double & degree)
{
	degreeOfDay = fmod(degree, 360);
	if (degreeOfDay > 180)	day = false;
	else day = true;
}

void GameObject::initialize(const Camera * cam, const glm::vec3 & _worldUp, const unsigned int _duration)
{
	duration = 60 * _duration;
	GameObject::camera = cam;
	WorldUp = _worldUp;
	degreeOfDay = 90.0;
	day = true;
}

void GameObject::stepSimulation(const double & _step)
{
	step = _step;
	degreeOfDay += (step * 360) / duration;
	degreeOfDay = fmod(degreeOfDay, 360);
	if (degreeOfDay > 180)	day = false;
	else day = true;
}


bool GameObject::isDay()
{
	return day;
}

GameObject::~GameObject()
{
}
