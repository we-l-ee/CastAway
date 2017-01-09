#pragma once
#include "Camera.h"
#include "pObject.h"

#include "stddef.hpp"

#include "glm\ext.hpp"

#include "OpenGL\include\gDebug\GLDebugDrawer.h"

#define CAMERA_HEIGHT 5

class pPerson :
	public Camera, pObject
{
	enum State{GROUND, FALL, JUMP, NEEL};
	int jump_count{ 0 };
	const int MAX_JUMP{ 2 };

	State state;

	shared_ptr< btDiscreteDynamicsWorld > btWorld;
	glm::vec3 gravity;
	glm::vec3 vel_u{ 0,0,0 };

	glm::vec3 vel_d{ 0,0,0 };
	glm::vec3 vel_f{ 0,0,0 };
	bool jump_forward;

	btVector3 dir_vel_f;
	
	inline glm::vec3 groundMovement(CameraMovement direction, double deltaTime, Speed sp = Speed::NORMAL);
	GLfloat vel;

	GLfloat currentYOffset;
	GLfloat neel_sin{ 90.0f };
	GLfloat rot{ -45.0 };
	inline void neeling(double deltaTime);

	inline void jump(double deltaTime);
	inline void fall(double deltaTime);
public:
	
	void calculateState();

	pPerson();
	pPerson( shared_ptr< btDiscreteDynamicsWorld > world );
	pPerson( shared_ptr< btDiscreteDynamicsWorld > world, glm::vec3 _pos);

	virtual void setMode(CameraMode mode);

	virtual glm::vec3 processMovement(CameraMovement direction, double deltaTime, Speed sp = Speed::NORMAL) override;

	void setbtWorld(shared_ptr< btDiscreteDynamicsWorld > world);
	
	void startJump(CameraMovement direction, Speed sp);

	void stepSimulation(double deltaTime);

	bool isFalling();
	bool onGround();
	bool canJump();

	~pPerson();


	#ifdef _DEBUG
		GLDebugDrawer gDebug;
	#endif

};

