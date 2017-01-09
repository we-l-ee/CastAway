#pragma once
#include "GameObject.h"
#include "Moveable.h"
#include <iostream>

#include "Camera.h"
#include "stddef.hpp"
#include <fstream>
#include "GExceptions.h"
#include "Light.h"

class PointLight :
	virtual public Moveable, public Light
{
	bool active{ true };
	

protected:
	PointLightData pointlight;
	StrengthData strengthData;

	inline void calculationAfterTranslate();

public:
	bool isActive() const;
	
	void setActive(bool s);
	void toggleActive();

	PointLight(const glm::vec3 & dis = glm::vec3(.0f), const glm::vec3 & color = glm::vec3(1.f));

	PointLight(const PointLightData & _pointlight, const glm::vec3 & dis = glm::vec3(.0f));

	PointLight(const std::string & obj);
	PointLight(const std::string & sub, const std::string & obj);
	PointLight(const std::string & obj, const glm::vec3 & dis);
	PointLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis);

	static PointLightData readLightFile(const std::string & path);

	PointLightData getPointLight() const;
	
	~PointLight();

	// Inherited via Light
	virtual void calculateLightening() override;
	virtual void increaseLightStrength() override;
	virtual void decreaseLightStrength() override;




	// Inherited via Light
	virtual glm::mat4 getLightViewMatrix() override;
	virtual glm::mat4 getLightProjMatrix() override;

};

