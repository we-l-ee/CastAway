#pragma once
#include <glm\ext.hpp>
#include "stddef.hpp"

class Moveable
{

protected:

	glm::vec3 position;
	Directions dir;

	glm::mat4 translation;
	glm::mat4 rotation;
	
	glm::quat quat;


	void pitch(float angle);
	void yaw(float angle);
	void roll(float angle);

	void pitchAround(float angle, const glm::vec3 & vector);
	void yawAround(float angle, const glm::vec3 & vector);
	void rollAround(float angle, const glm::vec3 & vector);

public:

	Moveable(const glm::vec3 & _pos = glm::vec3{ 0,0,0 } );

	virtual void reset( const glm::mat4 & matrix );
	virtual void reset( const glm::mat4 & trans, const glm::mat4 & rot );

	virtual void translate(glm::vec3 displacement);
	virtual void translate(float x, float y, float z);
	virtual void translateOn(float x, float y, float z, const Directions & _dir );

	virtual void rotate(float x, float y, float z);
	virtual void rotateAround(float x, float y, float z, const Directions & dir );


	glm::mat4 calculateModelMatrix();

	virtual ~Moveable()=default;
};

