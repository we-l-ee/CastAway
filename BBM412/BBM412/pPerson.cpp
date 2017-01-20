#include "pPerson.h"



pPerson::pPerson()
{
}

pPerson::pPerson( shared_ptr< btDiscreteDynamicsWorld >  world)
{
	btWorld = world;
}

pPerson::pPerson( shared_ptr< btDiscreteDynamicsWorld > world, glm::vec3 _pos ):Camera{_pos}
{
	btWorld = world;
}

void pPerson::setMode(CameraMode mode)
{
	if (mode == CameraMode::DEFAULT && cameraMode != CameraMode::DEFAULT)
	{
		calculateState();
	}
	cameraMode = mode;
}



glm::vec3 pPerson::processMovement(CameraMovement direction, double deltaTime, Speed sp)
{

	switch (cameraMode)
	{
	case CameraMode::DEFAULT:
		calculateViewMatrix();
		switch (state) 
		{
		case GROUND:
			return groundMovement(direction, deltaTime, sp);
		}

	case CameraMode::FLY:
		return Camera::processMovement(direction, deltaTime, sp);
	case CameraMode::GHOST:
		return Camera::processMovement(direction, deltaTime, sp);
	default:
		return{ 0,0,0 };

#ifdef _DEBUG_
		cout << "Start to end ray cast->" << start << "||" << end << endl;
		cout << "Start to hit->" << start << "||" << cb_f.m_hitPointWorld << endl;
		cout << "Normal->" << "||" << _cos << "|| HasHit:" << cb_f.hasHit() << endl;
#endif

#ifdef _DEBUG_
		if (btWorld == nullptr)
		{
			cout << "btWorld==NULL" << endl;
			break;
		}
#endif

		//this->setYOffset();

#if _DEBUG


		//cout <<"To ground->"<< start <<"||"<< cb_d.m_hitPointWorld << endl;
		//cout << "Before->" << position << "||" <<endl;
		//cout << "After->" << position << "||" <<  endl;

#endif

	}
}



glm::vec3 pPerson::groundMovement(CameraMovement direction, double deltaTime, Speed sp)
{
	btVector3 start, end; float _cos;
	CameraMovement  moves= CameraMovement::NO_MOVE;

	if( (int)(direction & CameraMovement::FORWARD) )
	{
		getForwardXZRay(5.0, start, end);
		btCollisionWorld::ClosestRayResultCallback cb_f(start, end);
		btWorld->rayTest(start, end, cb_f);
		_cos = glm::dot(
			glm::vec3(0, 1, 0),
			glm::normalize(pObject::btVector3_glm(cb_f.m_hitNormalWorld))
		);
		if (!cb_f.hasHit() || _cos > 0.6)
		{
			moves = moves | CameraMovement::FORWARD;
		}
	}

	if ((int)(direction & CameraMovement::BACKWARD))
	{
		getBackwardRay(5.0, start, end);
		btCollisionWorld::ClosestRayResultCallback cb_f(start, end);
		btWorld->rayTest(start, end, cb_f);
		_cos = glm::dot(
			glm::vec3(0, 1, 0),
			glm::normalize(pObject::btVector3_glm(cb_f.m_hitNormalWorld))
		);
		if (!cb_f.hasHit() || _cos > 0.6)
		{
			moves = moves | CameraMovement::BACKWARD;
		}
	}
	if((int)(direction & CameraMovement::RIGHT))
	{
		getRightRay(5.0, start, end);
		btCollisionWorld::ClosestRayResultCallback cb_f(start, end);
		btWorld->rayTest(start, end, cb_f);
		_cos = glm::dot(
			glm::vec3(0, 1, 0),
			glm::normalize(pObject::btVector3_glm(cb_f.m_hitNormalWorld))
		);
		if (!cb_f.hasHit() || _cos > 0.6)
		{
			moves = moves | CameraMovement::RIGHT;
		}
	}

	if((int)(direction & CameraMovement::LEFT))
	{
		getLeftRay(5.0, start, end);
		btCollisionWorld::ClosestRayResultCallback cb_f(start, end);
		btWorld->rayTest(start, end, cb_f);
		_cos = glm::dot(
			glm::vec3(0, 1, 0),
			glm::normalize(pObject::btVector3_glm(cb_f.m_hitNormalWorld))
		);
		if (!cb_f.hasHit() || _cos > 0.6)
		{
			moves = moves | CameraMovement::LEFT;
		}
	}


	this->getWorldDownRay(CAMERA_HEIGHT + 0.5, start, end);
	btCollisionWorld::ClosestRayResultCallback cb_d(start, end);
	btWorld->rayTest(start, end, cb_d);
	if( cb_d.hasHit() )
		position.y = cb_d.m_hitPointWorld.getY() + CAMERA_HEIGHT;
	else
	{
#ifdef _DEBUG
		cout << "Stat Fallin" << endl;
#endif
		if (direction == CameraMovement::FORWARD) { vel_f = forward; }
		state = FALL;
		fall(deltaTime);
	}

	return Camera::processMovement(moves, deltaTime, sp);
}

inline void pPerson::neeling(double deltaTime)
{
	double _rot = rot * deltaTime;
	neel_sin = neel_sin + _rot;
	position.y = currentYOffset + sin(glm::radians( neel_sin ) )*CAMERA_HEIGHT;
#ifdef _DEBUG
	cout << position.y <<"[e]"<<neel_sin<<"[e]"<<rot<< endl;
#endif

	if (neel_sin <= 45)
	{
		rot = +60;
	}
	if (neel_sin >= 90)
	{
		position.y = currentYOffset + CAMERA_HEIGHT;
		state = GROUND;
		neel_sin = 90.0f;
	}
}

inline void pPerson::calculateState()
{
	btVector3 start, end;

	this->getWorldDownRay(3.0, start, end);
	btCollisionWorld::ClosestRayResultCallback cb_d(start, end);
	btWorld->rayTest(start, end, cb_d);
	if (cb_d.hasHit())
	{
#ifdef _DEBUG
		cout << "Allready on the ground" << endl;
#endif
		position.y = cb_d.m_hitPointWorld.getY() + 2.80f;
	}
	else
	{
#ifdef _DEBUG
		cout << "State FALL" << endl;
#endif
		state = FALL;
	}
}

inline void pPerson::jump(double deltaTime)
{
	btVector3 start, end; float _cos;

	position += vel_u * deltaTime;
	vel_u += gravity*deltaTime;
	position += (vel_u*deltaTime) / 2;

	if (jump_forward)
	{
		start = pObject::glm_btVector3(position);
			//- dir_vel_f * 30;
		end = start + dir_vel_f * 5;

		btCollisionWorld::ClosestRayResultCallback cb_f(start, end);
		btWorld->rayTest(start, end, cb_f);
			_cos = glm::dot(
			glm::vec3(0, 1, 0),
			glm::normalize(pObject::btVector3_glm(cb_f.m_hitNormalWorld))
			);
		if (cb_f.hasHit() && _cos >=0.8)
		{

			vel_f = glm::vec3{ 0 };
			jump_forward = false;
		}
		else
		{
			position += vel_f * deltaTime;
		}
		this->getWorldDownRay(100.0, start, end);
		btCollisionWorld::ClosestRayResultCallback cb_d(start, end);
		btWorld->rayTest(start, end, cb_d);
		if (cb_d.hasHit() && position.y <= cb_d.m_hitPointWorld.getY() + 2.80)
		{
			jump_count = 0;
			vel_u = glm::vec3{ 0 };

			state = GROUND;
			vel_d = glm::vec3{ 0 };
			vel_f = glm::vec3{ 0 };
			position.y = cb_d.m_hitPointWorld.getY() + 2.80f;
			jump_forward = false;
		}
	}

	if (vel_u < glm::vec3{ 0 })
	{
		jump_count = 0;

		vel_u = glm::vec3{ 0 };
		state = FALL;
	}
}

inline void pPerson::fall(double deltaTime)
{
	btVector3 start, end;

	this->getWorldDownRay(100.0, start, end);
	btCollisionWorld::ClosestRayResultCallback cb_d(start, end);
	btWorld->rayTest(start, end, cb_d);

	position += vel_d * deltaTime;
	vel_d += gravity*deltaTime;
	position += (vel_d*deltaTime) / 2;

	if (jump_forward)
	{
		start = pObject::glm_btVector3(position);
			//- dir_vel_f*30;
		end = start + dir_vel_f * 5;

		btCollisionWorld::ClosestRayResultCallback cb_f(start, end);
		btWorld->rayTest(start, end, cb_f);
		if ( cb_f.hasHit() )
		{

			vel_f = glm::vec3{ 0 };
			jump_forward = false;
		}
		else
		{
			position += vel_f * deltaTime;
		}
		this->getWorldDownRay(100.0, start, end);
		btCollisionWorld::ClosestRayResultCallback cb_d2(start, end);
		btWorld->rayTest(start, end, cb_d2);
		if (cb_d2.hasHit() && position.y <= cb_d2.m_hitPointWorld.getY() + CAMERA_HEIGHT)
		{
			vel = glm::sqrt(glm::length2(vel_d) + glm::length2(vel_f));
#ifdef _DEBUG
			cout <<"total vel:"<< vel << endl;
#endif

			if (vel > 20)
			{
				state = NEEL;
				rot = -20 * (vel);
			}
			else
			{
				state = GROUND;
			}

			vel_d = glm::vec3{ 0 };
			vel_f = glm::vec3{ 0 };
			position.y = cb_d2.m_hitPointWorld.getY() + CAMERA_HEIGHT;
			jump_forward = false;
#ifdef _DEBUG
			cout << vel << endl;
#endif
			currentYOffset = cb_d2.m_hitPointWorld.getY();
		}
	}

	if ( cb_d.hasHit() && position.y <= cb_d.m_hitPointWorld.getY()+ CAMERA_HEIGHT)
	{
		vel = glm::sqrt(glm::length2(vel_d) + glm::length2(vel_f));
#ifdef _DEBUG
		cout << "total vel:" << vel << endl;
#endif


		if (vel > 20)
		{
			state = NEEL;
			rot = -20 * ( vel );
		}
		else
		{
			state = GROUND;
		}

		vel_d = glm::vec3{ 0 };
		vel_f = glm::vec3{ 0 };
		position.y = cb_d.m_hitPointWorld.getY() + CAMERA_HEIGHT;
		jump_forward = false;

		currentYOffset = cb_d.m_hitPointWorld.getY();
	}
}

void pPerson::stepSimulation(double deltaTime)
{
	if (cameraMode == CameraMode::DEFAULT)
	{
		switch (state)
		{
		case FALL:
			fall(deltaTime);
			break;
		case JUMP:
			jump(deltaTime);
			break;
		case NEEL:
			neeling(deltaTime);
			break;
		}
	}
}

bool pPerson::isFalling()
{
	return state==FALL;
}

bool pPerson::onGround()
{
	return state==GROUND;
}

bool pPerson::canJump()
{
	return jump_count < MAX_JUMP;
}



void pPerson::setbtWorld(shared_ptr<btDiscreteDynamicsWorld> world)
{
	btWorld = world;
	gravity = pObject::btVector3_glm( btWorld->getGravity() );
#ifdef _DEBUG
	cout << "gravity:" << gravity << endl;
#endif
}

pPerson::~pPerson()
{
}

void pPerson::startJump(CameraMovement direction, Speed sp)
{
	jump_count++;

	float k = 5.0;
	switch (sp) {
	case Speed::FAST:
		k = 10;
		break;
	case Speed::SLOW:
		k = 1;
		break;
	}
	state = JUMP;
	vel_u = glm::vec3{ 0,1,0 }*k;
	vel_f = this->getCurrentDirection(direction)*k;
	if (vel_f.x != 0 || vel_f.z != 0)
		jump_forward = true;
	dir_vel_f = pObject::glm_btVector3(glm::normalize(glm::vec3{ vel_f.x,0,vel_f.z } ));

	vel_d = glm::vec3{ 0 };

#ifdef _DEBUG
	cout << dir_vel_f << "[e]"<<jump_forward<<endl;
	//cout << vel_f << endl;
#endif
}


