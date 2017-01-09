#include "GameObject.h"

const Camera *GameObject::camera;
glm::vec3 GameObject::WorldUp;
double GameObject::step;
double GameObject::degreeOfDay;
unsigned int GameObject::duration;

GameObject::GameObject()
{
}

void GameObject::initialize(const Camera * cam, const glm::vec3 & _worldUp, const unsigned int _duration)
{
	duration = 60 * _duration;
	GameObject::camera = cam;
	WorldUp = _worldUp;
	degreeOfDay = 90.0;
}

void GameObject::stepSimulation(const double & _step)
{
	step = _step;
	degreeOfDay += (step * 360) / duration;
	degreeOfDay = fmod(degreeOfDay, 360);
}


GameObject::~GameObject()
{
}
