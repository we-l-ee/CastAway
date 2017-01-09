#include "DirecLight.h"

DirecLight::DirecLight(const glm::vec3 & dis, const glm::vec3 & color) :direclight{ glm::vec3{ 0,0,0 }, color },
strengthData{ direclight.ambient , direclight.diffuse , direclight.specular , 0.1 }, Moveable(dis)
{
}

DirecLight::DirecLight(const std::string & obj) : DirecLight(obj, obj, glm::vec3{ .0f })
{
}

DirecLight::DirecLight(const std::string & sub, const std::string & obj) : DirecLight(sub, obj, glm::vec3{ .0f })
{
}

DirecLight::DirecLight(const std::string & obj, const glm::vec3 & dis) : DirecLight(obj, obj, dis)

{
}

DirecLight::DirecLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis) : 
	Moveable(dis), direclight{ readLightFile("objects\\" + sub + "\\" + obj + ".light") },
	strengthData{ direclight.ambient , direclight.diffuse , direclight.specular , 0.1 }
{
}

bool DirecLight::isActive() const
{
	return active;
}

void DirecLight::setActive(bool s)
{
	active = s;

}

void DirecLight::toggleActive()
{
	active = !active;
}

DirecLightData DirecLight::getDirecLight() const
{
	return direclight;
}

void DirecLight::calculateDirecLight(const glm::vec3 & center)
{
	direclight.direction = center - glm::vec3{ camera->getViewMatrix()*glm::vec4{ Moveable::position,glm::vec1{ 1.0 } } };
}

void DirecLight::calculateDirecLightToCenter()
{

	direclight.direction = -glm::vec3{ camera->getViewMatrix()*glm::vec4{ Moveable::position,glm::vec1{ 1.0 } } };

}


DirecLight::~DirecLight()
{
}

void DirecLight::increaseLightStrength()
{
	if (direclight.ambient >= strengthData.ambient)
		return;

	direclight.ambient += strengthData.delta_ambient;
	direclight.diffuse += strengthData.delta_diffuse;
	direclight.specular += strengthData.delta_specular;

}

void DirecLight::decreaseLightStrength()
{
	if (direclight.ambient == glm::vec3{ .0f })
		return;

	direclight.ambient -= strengthData.delta_ambient;
	direclight.diffuse -= strengthData.delta_diffuse;
	direclight.specular -= strengthData.delta_specular;
	if (direclight.ambient < glm::vec3{ .0f })
	{
		direclight.ambient = glm::vec3{ 0.0f };
		direclight.diffuse = glm::vec3{ 0.0f };
		direclight.specular = glm::vec3{ 0.0f };
	}
}

void DirecLight::calculateLightening()
{
	calculateDirecLightToCenter();
}

DirecLightData DirecLight::readLightFile(const std::string &path) {

	std::ifstream file{ path };

	if (!file.is_open())
		throw GException("File can not open for readLightFile. Path:" + path + "\n");

	std::string cmd;
	file >> cmd;

	glm::vec3 dir;	 glm::vec3 color;
	glm::vec3 ambient;	glm::vec3 diffuse;	glm::vec3 specular;

	while (!file.eof()) {
		GLfloat p[3];

		if (cmd == "d") {
			file >> p[0] >> p[1] >> p[2];
			dir = glm::vec3{ p[0],p[1],p[2] };
		}
		else if (cmd == "c") {
			file >> p[0] >> p[1] >> p[2];
			color = glm::vec3{ p[0], p[1], p[2] };
		}
		else if (cmd == "a") {
			file >> p[0] >> p[1] >> p[2];
			ambient = glm::vec3{ p[0], p[1], p[2] };
		}
		else if (cmd == "d") {
			file >> p[0] >> p[1] >> p[2];
			diffuse = glm::vec3{ p[0], p[1], p[2] };
		}
		else if (cmd == "s") {
			file >> p[0] >> p[1] >> p[2];
			specular = glm::vec3{ p[0], p[1], p[2] };
		}

		file.ignore(1024, '\n');
		file >> cmd;
	}

	return DirecLightData{ dir , color , ambient , diffuse , specular };

}

glm::mat4 DirecLight::getLightViewMatrix()
{
	glm::vec3 forward = glm::normalize(position);
	return glm::lookAt(forward * 100, forward, GameObject::WorldUp);
}

glm::mat4 DirecLight::getLightProjMatrix()
{
	GLfloat near_plane = 1.0f, far_plane = 1000.0f;
	return glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
}
