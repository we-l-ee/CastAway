#include "SpotLight.h"
#include "glm\ext.hpp"



SpotLight::SpotLight(const glm::vec3 & dis, const glm::vec3 & color):spotlight{ glm::vec3{.0f}, glm::vec3{ .0f, .0f , -1.0f }, color } , 
									strengthData{ spotlight.ambient , spotlight.diffuse , spotlight.specular , 0.1} , Moveable(dis)
{
}

SpotLight::SpotLight(const std::string & obj) : SpotLight(obj, obj, glm::vec3{.0f})  
{
}

SpotLight::SpotLight(const std::string & sub, const std::string & obj) : SpotLight(sub, obj, glm::vec3{ .0f })
{
}

SpotLight::SpotLight(const std::string & obj, const glm::vec3 & dis): SpotLight(obj , obj, dis)
																		
{
}

SpotLight::SpotLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis) : Moveable(dis), spotlight{ readLightFile("objects\\" + sub + "\\" + obj + ".light") },
strengthData{ spotlight.ambient , spotlight.diffuse , spotlight.specular , 0.1 }
{

}

bool SpotLight::isActive() const
{
	return active;
}

void SpotLight::setActive(bool s)
{
	active = s;
}

void SpotLight::toggleActive()
{
	active = !active;
}



SpotLightData SpotLight::getSpotLight() const
{
	return spotlight;
}


SpotLight::~SpotLight()
{
}

void SpotLight::increaseLightStrength()
{
	if (spotlight.ambient >= strengthData.ambient)
		return;

	spotlight.ambient += strengthData.delta_ambient;
	spotlight.diffuse += strengthData.delta_diffuse;
	spotlight.specular += strengthData.delta_specular;
}

void SpotLight::decreaseLightStrength()
{
	if (spotlight.ambient == glm::vec3{ .0f })
		return;

	spotlight.ambient -=  strengthData.delta_ambient;
	spotlight.diffuse -= strengthData.delta_diffuse;
	spotlight.specular -= strengthData.delta_specular;
	if (spotlight.ambient < glm::vec3{ .0f })
	{
		spotlight.ambient = glm::vec3{ 0.0f };
		spotlight.diffuse = glm::vec3{ 0.0f };
		spotlight.specular = glm::vec3{ 0.0f };
	}
}

void SpotLight::calculateLightening()
{

		spotlight.position = glm::vec3(GameObject::camera->getViewMatrix() * glm::vec4(Moveable::position, glm::vec1{ 1.0f }));
		spotlight.direction = //glm::vec4{ 5.0f,5.0f,5.0f,1.0f };
			glm::mat3(GameObject::camera->getViewMatrix()) * (Moveable::dir.Forward);
		//glm::vec3( camera->getViewMatrix() * glm::vec4{ 5.0f,5.0f,5.0f,1.0f } );
		//cout << spotlight.direction.x << "," << spotlight.direction.y << "," << spotlight.direction.z << endl;

}

SpotLightData SpotLight::readLightFile(const std::string & path) {
	std::ifstream file{ path };

	if (!file.is_open())
		throw GException("File can not open for readLightFile. Path:"+ path+"\n");

	std::string cmd;
	file >> cmd;

	glm::vec3 pos;	 glm::vec3 color;	glm::vec3 dir;
	glm::vec3 ambient;	glm::vec3 diffuse;	glm::vec3 specular;
	GLfloat constant; GLfloat linear; GLfloat quadratic;
	GLfloat cutOff; GLfloat outerCutOff;

	while (!file.eof()) {
		GLfloat p[3];

		if (cmd == "p") {
			file >> p[0] >> p[1] >> p[2];
			pos = glm::vec3{ p[0],p[1],p[2] };
		}
		else if (cmd == "c") {
			file >> p[0] >> p[1] >> p[2];
			color = glm::vec3{ p[0], p[1], p[2] };
		}
		else if (cmd == "di") {
			file >> p[0] >> p[1] >> p[2];
			dir = glm::vec3{ p[0], p[1], p[2] };
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
		else if (cmd == "co") {
			file >> p[0];
			constant = p[0];
		}
		else if (cmd == "q") {
			file >> p[0];
			quadratic = p[0];
		}
		else if (cmd == "l") {
			file >> p[0];
			linear = p[0];
		}
		else if (cmd == "cu") {
			file >> p[0];
			cutOff = p[0];
		}
		else if (cmd == "oc") {
			file >> p[0];
			outerCutOff = p[0];
		}

		file.ignore(1024, '\n');
		file >> cmd;
	}

	return SpotLightData{ pos , dir , color, cutOff , outerCutOff  , ambient , diffuse , specular , constant , linear ,quadratic };

}

glm::mat4 SpotLight::getLightViewMatrix()
{
	return glm::mat4();
}

glm::mat4 SpotLight::getLightProjMatrix()
{
	return glm::mat4();
}
