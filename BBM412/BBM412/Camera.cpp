#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 _pos, glm::vec3 up , GLfloat yaw , GLfloat pitch  ) : 
	MouseSensitivity(SENSITIVTY), Yaw(yaw), Pitch(pitch)
{
	position = _pos;
	dir.Up = up;

	projMatrix = glm::perspective(glm::radians(45.0f), 2.0f, 0.1f, 2000.0f);

}

Camera::~Camera()
{
}

void Camera::getForwardXZRay(float to, btVector3 & start, btVector3 & end)
{
	start = btVector3(position.x, position.y, position.z);
	switch (cameraMode)
	{
	case CameraMode::DEFAULT:
	case CameraMode::GHOST:
		end = start +
			btVector3(forward.x, forward.y, forward.z)
			//btVector3(dir.Forward.x, dir.Forward.y, dir.Forward.z)
			* to;
		break;
	case CameraMode::FLY:
		end = start + btVector3(fly.Forward.x, fly.Forward.y, fly.Forward.z) * to;
		break;
	}
}

void Camera::getForwardXYZRay(float to, btVector3 & start, btVector3 & end)
{
	start = btVector3(position.x, position.y, position.z);
	switch (cameraMode)
	{
	case CameraMode::DEFAULT:
	case CameraMode::GHOST:
		end = start +
			btVector3(dir.Forward.x, dir.Forward.y, dir.Forward.z)
			* to;
		break;
	case CameraMode::FLY:
		end = start + btVector3(fly.Forward.x, fly.Forward.y, fly.Forward.z) * to;
		break;
	}
}

void Camera::getBackwardRay(float to, btVector3 & start, btVector3 & end)
{
	start = btVector3(position.x, position.y, position.z);
	switch (cameraMode)
	{
	case CameraMode::DEFAULT:
	case CameraMode::GHOST:
		end = start +
			btVector3(-forward.x, -forward.y, -forward.z)
			//btVector3(dir.Forward.x, dir.Forward.y, dir.Forward.z)
			* to;
		break;
	case CameraMode::FLY:
		end = start + btVector3(fly.Forward.x, fly.Forward.y, fly.Forward.z) * to;
		break;
	}
}

void Camera::getRightRay(float to, btVector3 & start, btVector3 & end)
{
	start = btVector3(position.x, position.y, position.z);
	switch (cameraMode)
	{
	case CameraMode::DEFAULT:
	case CameraMode::GHOST:
		end = start +
			btVector3(dir.Right.x, dir.Right.y, dir.Right.z)
			//btVector3(dir.Forward.x, dir.Forward.y, dir.Forward.z)
			* to;
		break;
	case CameraMode::FLY:
		end = start + btVector3(fly.Forward.x, fly.Forward.y, fly.Forward.z) * to;
		break;
	}
}

void Camera::getLeftRay(float to, btVector3 & start, btVector3 & end)
{
	start = btVector3(position.x, position.y, position.z);
	switch (cameraMode)
	{
	case CameraMode::DEFAULT:
	case CameraMode::GHOST:
		end = start +
			btVector3(-dir.Right.x, -dir.Right.y, -dir.Right.z)
			//btVector3(dir.Forward.x, dir.Forward.y, dir.Forward.z)
			* to;
		break;
	case CameraMode::FLY:
		end = start + btVector3(fly.Forward.x, fly.Forward.y, fly.Forward.z) * to;
		break;
	}
}

void Camera::getWorldDownRay(float to, btVector3 & start, btVector3 & end)
{
	start = btVector3(position.x, position.y, position.z);
	end = start + btVector3(-dir.Up.x, -dir.Up.y, -dir.Up.z) * to;
}

void Camera::getWorldUpRay(float to, btVector3 & start, btVector3 & end)
{
	start = btVector3(position.x, position.y, position.z);
	end = start + btVector3(dir.Up.x, dir.Up.y, dir.Up.z) * to;
}

glm::mat4 Camera::getSeaReflectedView()
{
	glm::vec3 forward{
		cos(glm::radians(this->Yaw)) * cos(glm::radians(-this->Pitch)),
		sin(glm::radians(-this->Pitch)),
		sin(glm::radians(this->Yaw)) * cos(-glm::radians(this->Pitch))
	};
	forward = glm::reflect(dir.Forward, glm::vec3{ 0,1,0 });
	glm::vec3 position = glm::vec3{ this->position.x, -this->position.y, this->position.z };
	position = glm::mat3(glm::scale(glm::vec3{ 1,-1,1 }))* this->position;

	return glm::lookAt(position, position + forward, this->dir.Up);
}

glm::mat4 Camera::getViewMatrix() const
{
	return viewMatrix;
}

glm::mat4 Camera::getProjMatrix() const
{
	return projMatrix;
}

glm::mat4 Camera::getViewProjMatrix() const
{
	return projMatrix*viewMatrix;
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

glm::vec3 Camera::getForwardDirection() const
{
	switch (cameraMode) {
	case CameraMode::DEFAULT:
	case CameraMode::GHOST:
		return dir.Forward;
	case CameraMode::FLY:
		return fly.Forward;
	}
}

glm::vec3 Camera::getRightDirection() const
{
	switch (cameraMode) {
	case CameraMode::DEFAULT:
	case CameraMode::GHOST:
		return dir.Right;
	case CameraMode::FLY:
		return fly.Right;
	}
}

glm::vec3 Camera::getUpDirection() const
{
	switch (cameraMode) {
	case CameraMode::DEFAULT:
	case CameraMode::GHOST:
		return dir.Up;
	case CameraMode::FLY:
		return fly.Up;
	}
}

Directions Camera::getDirections() const
{
	switch (cameraMode) {
		case CameraMode::DEFAULT:
		case CameraMode::GHOST:
			return dir;
		case CameraMode::FLY:
			return fly;
	}
}

CameraMode Camera::getCameraMode() const
{
	return cameraMode;
}

glm::vec3 Camera::getCurrentDirection(CameraMovement directions)
{
	glm::vec3 deltaPos{ 0,0 ,0};
	if ( (int)(directions & CameraMovement::FORWARD) )
		deltaPos += forward ;
	if ( (int)(directions & CameraMovement::BACKWARD) )
		deltaPos -= forward ;
	if ((int) (directions & CameraMovement::LEFT) )
		deltaPos -= dir.Right;
	if ((int) (directions & CameraMovement::RIGHT))
		deltaPos += dir.Right;

	return deltaPos;
}

void Camera::calculateViewMatrix()
{
	switch (cameraMode) {
	case CameraMode::DEFAULT:
	case CameraMode::GHOST:
		updateCameraVectors();
		viewMatrix = glm::lookAt(position, position + dir.Forward, dir.Up);
		break;
	case CameraMode::FLY:
		viewMatrix = glm::lookAt(position, position + fly.Forward, fly.Up);
		break;
	}
}


glm::vec3 Camera::processMovement(CameraMovement direction, double deltaTime, Speed sp)
{
	float velocity= velocity_normal;
	switch (sp) {
	case Speed::FAST:
		velocity = velocity_fast;
		break;
	case Speed::SLOW:
		velocity = velocity_slow;
		break;
	}
	glm::vec3 deltaPos{ 0.0 };

	double delta = velocity * deltaTime;
	double rotate = 45 * deltaTime;

	switch (cameraMode) {
	case CameraMode::DEFAULT:
		if((int)(direction & CameraMovement::FORWARD))
			deltaPos += forward * delta;
		if((int)(direction & CameraMovement::BACKWARD))
			deltaPos -= forward * delta;
		if((int)(direction & CameraMovement::LEFT))
			deltaPos -= dir.Right * delta;
		if((int)(direction & CameraMovement::RIGHT))
			deltaPos += dir.Right * delta;
		
		position += deltaPos;
		return deltaPos;

	case CameraMode::GHOST:
		if((int)(direction & CameraMovement::FORWARD))
			deltaPos += forward * delta;
		if((int)(direction &  CameraMovement::BACKWARD))
			deltaPos -= forward * delta;
		if((int)(direction &  CameraMovement::LEFT))
			deltaPos -= dir.Right * delta;
		if((int)(direction &  CameraMovement::RIGHT))
			deltaPos += dir.Right * delta;

		if((int)(direction &  CameraMovement::UP))
			deltaPos += dir.Up*delta;
		if((int)(direction &  CameraMovement::DOWN))
			deltaPos -= dir.Up*delta;

		position += deltaPos;
		return deltaPos;

	case CameraMode::FLY:
		if((int)(direction &   CameraMovement::FORWARD))
			deltaPos += fly.Forward * delta;
		if((int)(direction &   CameraMovement::BACKWARD))
			deltaPos -= fly.Forward* delta;
		if((int)(direction &   CameraMovement::LEFT))
			deltaPos -= fly.Right*delta;
		if((int)(direction &   CameraMovement::RIGHT))
			deltaPos += fly.Right*delta;

		if((int)(direction &   CameraMovement::UP))
			deltaPos += fly.Up *delta;
		if((int)(direction &   CameraMovement::DOWN))
			deltaPos -= fly.Up *delta;

		if((int)(direction &  CameraMovement::ROLL_POS) )
			roll(+rotate);
		if((int)(direction &  CameraMovement::ROLL_NEG))
			roll(-rotate);

		if((int)(direction &  CameraMovement::YAW_POS))
			yaw(-rotate);
		if((int)(direction &  CameraMovement::YAW_NEG))
			yaw(+rotate);

		if((int)(direction &  CameraMovement::PITCH_POS))
			pitch(+rotate);
		if((int)(direction &  CameraMovement::PITCH_NEG))
			pitch(-rotate);

		position += deltaPos;
		return deltaPos;

	}
}

void Camera::processMouseMovement( double xoffset,  double yoffset, bool constrainPitch )
{
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

#if (DEBUG_LVL>=2)
	std::cout << Yaw << "," << Pitch << std::endl;
#endif

	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
		this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
		this->Pitch = -89.0f;
	}
	
}

void Camera::setYOffset(const float & yOffset)
{
	position.y = yOffset;
}

void Camera::pitch(float angle)
{

	glm::quat q = glm::angleAxis(
		glm::radians( -angle ), fly.Right
	);

	fly.Forward = glm::rotate(q, fly.Forward);
	fly.Up = glm::rotate(q, fly.Up);
#if (DEBUG_LVL>=2)
	std::cout << "Pitch:" << std::endl;
	std::cout << fly.Forward.x <<","<< fly.Forward.y<<","<< fly.Forward.z<< std::endl;
	std::cout << fly.Right.x << "," << fly.Right.y << "," << fly.Right.z << std::endl;
	std::cout << fly.Up.x << "," << fly.Up.y << "," << fly.Up.z << std::endl;

#endif

	quat = q*quat;

}

void Camera::yaw(float angle)
{
	glm::quat q = glm::angleAxis(
		glm::radians(angle), fly.Up
	);

	fly.Forward = glm::rotate(q, fly.Forward);
	fly.Right = glm::rotate(q, fly.Right);
#if (DEBUG_LVL>=2)
	std::cout << "Yaw:" << std::endl;
	std::cout << fly.Forward.x << "," << fly.Forward.y << "," << fly.Forward.z << std::endl;
	std::cout << fly.Right.x << "," << fly.Right.y << "," << fly.Right.z << std::endl;
	std::cout << fly.Up.x << "," << fly.Up.y << "," << fly.Up.z << std::endl;

#endif

	quat = q*quat;

}

void Camera::roll(float angle)
{
	glm::quat q = glm::angleAxis(
		glm::radians(angle), fly.Forward
	);

	fly.Right = glm::rotate(q, fly.Right);
	fly.Up = glm::rotate(q, fly.Up);
#if (DEBUG_LVL>=2)
	std::cout << "Roll:" << std::endl;
	std::cout << fly.Forward.x << "," << fly.Forward.y << "," << fly.Forward.z << std::endl;
	std::cout << fly.Right.x << "," << fly.Right.y << "," << fly.Right.z << std::endl;
	std::cout << fly.Up.x << "," << fly.Up.y << "," << fly.Up.z << std::endl;

#endif

	quat = q*quat;

}

void Camera::updateCameraVectors()
{
	forward.x =
		cos(glm::radians(this->Yaw));
	forward.z =
		sin(glm::radians(this->Yaw));

	glm::vec3 forward{
		cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch)),
		sin(glm::radians(this->Pitch)),
		sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch))
	};

#if (DEBUG_LVL>=2)
#endif

	dir.Forward = glm::normalize(forward);

	dir.Right = glm::normalize(glm::cross(dir.Forward, dir.Up));


}
