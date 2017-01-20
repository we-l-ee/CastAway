#pragma once
#include <vector>

#include <glew.h>
#include <glm/ext.hpp>

#include "stddef.hpp"


const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.10f;



class Camera
{


public:

	
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = 90, GLfloat pitch = 0);

	virtual ~Camera();

	virtual void setMode(CameraMode mode) { this->cameraMode = mode; }

	void getForwardXZRay(float to, btVector3 & start, btVector3 & end);
	void getForwardXYZRay(float to, btVector3 & start, btVector3 & end);

	void getBackwardRay(float to, btVector3 & start, btVector3 & end);

	void getRightRay(float to, btVector3 & start, btVector3 & end);
	void getLeftRay(float to, btVector3 & start, btVector3 & end);

	void getWorldDownRay(float to, btVector3 & start, btVector3 & end);
	void getWorldUpRay(float to, btVector3 & start, btVector3 & end);

	//glm::mat4 getReflectedView( const glm::vec3 & normal );
	//glm::mat4 getSeaReflectedView();

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjMatrix()	const;
	glm::mat4 getViewProjMatrix()	const;
	glm::vec3 getPosition()	const;
	


	glm::vec3 getForwardDirection() const;
	glm::vec3 getRightDirection() const;
	glm::vec3 getUpDirection() const;

	Directions getDirections() const;

	CameraMode getCameraMode() const;


	glm::vec3 getCurrentDirection(CameraMovement directions);
	void calculateViewMatrix();


	virtual glm::vec3 processMovement(CameraMovement direction, double deltaTime, Speed sp= Speed::NORMAL);
	virtual void processMouseMovement( double xoffset, double yoffset, bool constrainPitch = true);

protected:

	void setYOffset(const float & yOffset);

	CameraMode cameraMode{ CameraMode::DEFAULT };

	glm::vec3 position;

	glm::vec3 forward{ 0.0,0.0, -1.0 };
private:

	void pitch(float angle);
	void yaw(float angle);
	void roll(float angle);

	void updateCameraVectors();
	
	glm::quat quat;

private:
	GLfloat MouseSensitivity;
	GLfloat RotationSensivity{ 45.0f };

	float velocity_normal{ 3.0f };
	float velocity_fast{ 10.0f };
	float velocity_slow{ 1.5f };

	glm::mat4 projMatrix;
	glm::mat4 viewMatrix;

	Directions dir;

	GLfloat Yaw;
	GLfloat Pitch;

	Directions fly;

};
