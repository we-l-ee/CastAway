#pragma once
#include "pObject.h"
#include "stddef.hpp"

class pDynamicObject : public pObject
{


protected:

	btDefaultMotionState* motionState;
	btRigidBody* rigidBody;
	btCollisionShape* collisionShape;

protected:

	void constructRigidBody();

public:
	pDynamicObject();


#ifdef _DEBUG
	void drawRigidBody(const btTransform & mvp, const btVector3 &color);
#endif
	


	~pDynamicObject();
};

