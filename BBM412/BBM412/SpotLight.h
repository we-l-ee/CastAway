#pragma once
#include "Light.h"
#include "Moveable.h"
#include "stddef.hpp"
#include "Camera.h"
#include <fstream>
#include "GExceptions.h"


class SpotLight :
	virtual public Moveable, public Light
{


protected:
	//The real position that will be used when calculating lightening. It is a dot in a aproximatly the center of the object.

	SpotLightData spotlight;
	float current_strength;
	float delta_strength;

	StrengthData strengthData;
	
	
public:

	SpotLight(const glm::vec3 & dis = glm::vec3(.0f), const glm::vec3 & color = glm::vec3(1.f));
	SpotLight(const std::string & obj);
	SpotLight(const std::string & sub,const std::string & obj);
	SpotLight(const std::string & obj, const glm::vec3 & dis);
	SpotLight(const std::string & dis, const std::string & obj, const glm::vec3 & color);

	SpotLightData getSpotLight() const;

	// Inherited via Light
	virtual void calculateLightening() override;
	virtual void increaseLightStrength() override;
	virtual void decreaseLightStrength() override;

	~SpotLight();

	static SpotLightData readLightFile(const std::string & path);

	// Inherited via Light
	virtual glm::mat4 getLightViewMatrix() override;
	virtual glm::mat4 getLightProjMatrix() override;
};

