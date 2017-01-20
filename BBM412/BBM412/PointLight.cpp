#include "PointLight.h"



//const Camera * Object::camera;

PointLight::PointLight(const glm::vec3 & dis, const glm::vec3 & color):
	Moveable(dis),
	pointlight{glm::vec3{ 0,0,0 }, color },
	strengthData{ pointlight.ambient,pointlight.diffuse,pointlight.specular , 0.1 }
{
}

PointLight::PointLight(const PointLightData & _pointlight, const glm::vec3 & dis):
	Moveable(dis),pointlight(_pointlight), strengthData{ pointlight.ambient, pointlight.diffuse , pointlight.specular , 0.1 }
	, current_strength(1.0f), delta_strength(0.1f)
{
}

PointLight::PointLight(const std::string & obj) : PointLight(obj, obj, glm::vec3{ .0f })
{
}

PointLight::PointLight(const std::string & dis, const std::string & obj) : PointLight(dis, obj, glm::vec3{ .0f })
{
}

PointLight::PointLight(const std::string & obj, const glm::vec3 & dis) : PointLight( obj , obj , dis )
{
}

PointLight::PointLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis) : 
	Moveable(dis), pointlight(readLightFile("objects\\" + sub + "\\" + obj + ".light") ), 
	strengthData{ pointlight.ambient, pointlight.diffuse , pointlight.specular , 0.1f },current_strength(1.0f),delta_strength(0.1f)
{
}



inline void PointLight::calculationAfterTranslate()
{
}

PointLightData PointLight::getPointLight() const
{
	return pointlight;
}


PointLight::~PointLight()
{
}


void PointLight::calculateLightening()
{
	glm::vec3 pos = glm::vec3{
		camera->getViewMatrix() * glm::vec4( Moveable::position, glm::vec1{ 1.0f } )
	};
	pointlight.position = pos;

}


void PointLight::increaseLightStrength()
{
	if (pointlight.ambient >= strengthData.ambient)
		return;

	pointlight.ambient += strengthData.delta_ambient;
	pointlight.diffuse += strengthData.delta_diffuse;
	pointlight.specular += strengthData.delta_specular;
}

void PointLight::decreaseLightStrength()
{
	if (pointlight.ambient == glm::vec3{ .0f })
		return;

	pointlight.ambient -= strengthData.delta_ambient;
	pointlight.diffuse -= strengthData.delta_diffuse;
	pointlight.specular -= strengthData.delta_specular;
	if (pointlight.ambient < glm::vec3{ .0f })
	{
		pointlight.ambient = glm::vec3{ 0.0f };
		pointlight.diffuse = glm::vec3{ 0.0f };
		pointlight.specular = glm::vec3{ 0.0f };
	}
}

glm::mat4 PointLight::getLightViewMatrix()
{
	return glm::mat4();
}

glm::mat4 PointLight::getLightProjMatrix()
{
	return glm::mat4();
}

PointLightData PointLight::readLightFile(const std::string & path) {
	std::ifstream file{ path };

	if (!file.is_open())
		throw GException("File can not open for readLightFile. Path:" + path + "\n");

	std::string cmd;
	file >> cmd;

	glm::vec3 pos;	 glm::vec3 color;
	glm::vec3 ambient;	glm::vec3 diffuse;	glm::vec3 specular;
	GLfloat constant; GLfloat linear; GLfloat quadratic;

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

		file.ignore(1024, '\n');
		file >> cmd;
	}

	return PointLightData{ pos , color , ambient , diffuse , specular , constant , linear ,quadratic };

}