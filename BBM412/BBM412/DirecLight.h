#pragma once
#include <fstream>
#include "Light.h"
#include "Moveable.h"
#include "Camera.h"
#include "stddef.hpp"
#include "GExceptions.h"
#include "Light.h"


class DirecLight : public virtual Moveable, public Light
{

	bool active{ true };

protected:

	DirecLightData direclight;
	StrengthData strengthData;

	void calculateDirecLight(const glm::vec3 & center);
	void calculateDirecLightToCenter();

public:
	DirecLight(const glm::vec3 & dis = glm::vec3(.0f), const glm::vec3 & color = glm::vec3(1.f));
	DirecLight(const std::string & obj);

	DirecLight(const std::string & sub, const std::string & obj);
	DirecLight(const std::string & obj, const glm::vec3 & dis);
	DirecLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis);

	bool isActive() const;

	void setActive(bool s);
	void toggleActive();

	DirecLightData getDirecLight() const;


	static DirecLightData readLightFile(const std::string &path);

	~DirecLight();

	// Inherited via Light
	virtual void increaseLightStrength() override;
	virtual void decreaseLightStrength() override;
	virtual void calculateLightening() override;

	virtual glm::mat4 getLightViewMatrix() override;
	virtual glm::mat4 getLightProjMatrix() override;

};

