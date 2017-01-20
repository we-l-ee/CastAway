#pragma once
#include "pObject.h"
#include "stddef.hpp"

class pDynamicObject : public pObject
{



public:
	pDynamicObject();


#ifdef _DEBUG
	void drawRigidBody(const btTransform & mvp, const btVector3 &color);
#endif
	


	~pDynamicObject();
};

