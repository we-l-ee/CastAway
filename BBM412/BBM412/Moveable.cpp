#include "Moveable.h"
#include <iostream>

void Moveable::pitch(float angle)
{
	glm::quat q = glm::angleAxis(
		glm::radians(-angle), dir.Right
	);

	dir.Forward = glm::rotate(q, dir.Forward);
	dir.Up = glm::rotate(q, dir.Up);
#if (DEBUG_LVL>=2)
	std::cout << "Pitch:" << std::endl;
	std::cout << dir.Forward.x << "," <<	dir.Forward.y << "," << dir.Forward.z << std::endl;
	std::cout << dir.Right.x << "," <<		dir.Right.y << "," <<	dir.Right.z << std::endl;
	std::cout << dir.Up.x << "," <<			dir.Up.y << "," <<		dir.Up.z << std::endl;

#endif
	quat = q*quat;
}

void Moveable::yaw(float angle)
{
	glm::quat q = glm::angleAxis(
		glm::radians(angle), dir.Up
	);

	dir.Forward = glm::rotate(q, dir.Forward);
	dir.Right = glm::rotate(q, dir.Right);
#if (DEBUG_LVL>=2)
	std::cout << "Yaw:" << std::endl;
	std::cout << dir.Forward.x << "," << dir.Forward.y << "," << dir.Forward.z << std::endl;
	std::cout << dir.Right.x << "," << dir.Right.y << "," << dir.Right.z << std::endl;
	std::cout << dir.Up.x << "," << dir.Up.y << "," << dir.Up.z << std::endl;

#endif
	quat = q*quat;

}

void Moveable::roll(float angle)
{
	glm::quat q = glm::angleAxis(
		glm::radians(angle), dir.Forward
	);

	dir.Right = glm::rotate(q, dir.Right);
	dir.Up = glm::rotate(q, dir.Up);
#if (DEBUG_LVL>=2)
	std::cout << "Roll:" << std::endl;
	std::cout << dir.Forward.x << "," << dir.Forward.y << "," << dir.Forward.z << std::endl;
	std::cout << dir.Right.x << "," << dir.Right.y << "," << dir.Right.z << std::endl;
	std::cout << dir.Up.x << "," << dir.Up.y << "," << dir.Up.z << std::endl;

#endif
	quat = q*quat;

}

void Moveable::pitchAround(float angle, const glm::vec3 & vector)
{
	glm::quat q = glm::angleAxis(
		glm::radians(-angle), vector
	);

	dir.Forward = glm::rotate(q, dir.Forward);
	dir.Up = glm::rotate(q, dir.Up);
	dir.Right = glm::normalize(glm::cross( dir.Up, dir.Forward ));

#if (DEBUG_LVL>=2)
	std::cout << "Pitch:" << std::endl;
	std::cout << dir.Forward.x << "," << dir.Forward.y << "," << dir.Forward.z << std::endl;
	std::cout << dir.Right.x << "," << dir.Right.y << "," << dir.Right.z << std::endl;
	std::cout << dir.Up.x << "," << dir.Up.y << "," << dir.Up.z << std::endl;

#endif

	quat = q*quat;

}

void Moveable::yawAround(float angle, const glm::vec3 & vector)
{
	glm::quat q = glm::angleAxis(
		glm::radians(angle), vector
	);

	dir.Forward = glm::rotate(q, dir.Forward);
	dir.Right = glm::rotate(q, dir.Right);
	dir.Up = glm::normalize(glm::cross(dir.Forward, dir.Right));

#if (DEBUG_LVL>=2)
	std::cout << "Yaw:" << std::endl;
	std::cout << dir.Forward.x << "," << dir.Forward.y << "," << dir.Forward.z << std::endl;
	std::cout << dir.Right.x << "," << dir.Right.y << "," << dir.Right.z << std::endl;
	std::cout << dir.Up.x << "," << dir.Up.y << "," << dir.Up.z << std::endl;

#endif
	quat = q*quat;

}

void Moveable::rollAround(float angle, const glm::vec3 & vector)
{
	glm::quat q = glm::angleAxis(
		glm::radians(angle), dir.Forward
	);

	dir.Right = glm::rotate(q, dir.Right);
	dir.Up = glm::rotate(q, dir.Up);
	dir.Forward = glm::normalize(glm::cross(dir.Right, dir.Up));

#if (DEBUG_LVL>=2)
	std::cout << "Roll:" << std::endl;
	std::cout << dir.Forward.x << "," << dir.Forward.y << "," << dir.Forward.z << std::endl;
	std::cout << dir.Right.x << "," << dir.Right.y << "," << dir.Right.z << std::endl;
	std::cout << dir.Up.x << "," << dir.Up.y << "," << dir.Up.z << std::endl;

#endif
	quat = q*quat;

}

Moveable::Moveable(const glm::vec3 & _pos)
{
	
	position = _pos;
	translation = glm::translate(position);

}

/*@param matrix
It takes inverse camera view matrix to reset by the current viewing.
*/
void Moveable::reset( const glm::mat4 & matrix)
{
	rotation = matrix * rotation;
	translation = matrix * translation;

	this->dir = Directions{
		glm::vec3(rotation*glm::vec4{ 0.0,0.0,-1.0,0.0 }),
		glm::vec3(rotation*glm::vec4{ 0.0,1.0,0.0,0.0 }),
		glm::vec3(rotation*glm::vec4{ 1.0,0.0,0.0,0.0 })
	};

	this->position = translation * glm::vec4(0, 0, 0, 1);

	this->quat = glm::normalize( glm::toQuat(rotation) );

}

void Moveable::reset(const glm::mat4 & trans, const glm::mat4 & rot)
{

	this->dir = Directions{
		glm::vec3(rot*glm::vec4{ 0.0,0.0,-1.0,0.0 }),
		glm::vec3(rot*glm::vec4{ 0.0,1.0,0.0,0.0 }),
		glm::vec3(rot*glm::vec4{ 1.0,0.0,0.0,0.0 })
	};

	this->position = trans * glm::vec4(0, 0, 0, 1);

	this->quat = glm::normalize(glm::toQuat(rot));

	translation = trans;	//glm::translate(this->position);
	rotation = rot;			//glm::mat4_cast(this->quat);

}

void Moveable::translate(glm::vec3 displacement)
{

	position += displacement;
	translation = glm::translate(position);

}

void Moveable::translate(float x, float y, float z)
{
	std::cout <<"Moveable::direction()\n"<< x << "," << y << "," << z << std::endl;

	if (z != 0) position += dir.Forward*z;
	if (y != 0) position += dir.Up*y;
	if (x != 0) position += dir.Right*x;

	translation = glm::translate( position );
}

void Moveable::translateOn(float x, float y, float z, const Directions & _dir)
{
	if (z != 0) position += _dir.Forward*z;
	if (y != 0) position += _dir.Up*y;
	if (x != 0) position += _dir.Right*x;

	translation = glm::translate(position);
}

void Moveable::rotate(float x, float y, float z)
{
	//std::cout << "Moveable::rotate()\n" << x << "," << y << "," << z << std::endl;
	bool f = false;

	if (x != 0) { pitch(x); f = true; }
	if (y != 0) { yaw(y);  f = true; }
	if (z != 0) { roll(z); f = true; }

	if ( true )
	{
		quat = glm::normalize(quat);
		rotation = glm::mat4_cast(quat);
	}

}

void Moveable::rotateAround(float x, float y, float z, const Directions & _dir)
{

	bool f = false;

	if (x != 0) { pitchAround(x,_dir.Right); f = true; }
	if (y != 0) { yawAround(y,_dir.Up);  f = true; }
	if (z != 0) { rollAround(z,_dir.Forward); f = true; }

	if (true)
	{
		quat = glm::normalize(quat);
		rotation = glm::mat4_cast(quat);
	}
}

glm::mat4 Moveable::calculateModelMatrix()
{
	return translation*rotation;
}
