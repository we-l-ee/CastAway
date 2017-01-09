#include "pDynamicObject.h"



pDynamicObject::pDynamicObject()
{

}


void pDynamicObject::constructRigidBody()
{

}

#ifdef _DEBUG
void pDynamicObject::drawRigidBody(const btTransform & mvp, const btVector3 &color)
{
	world->debugDrawObject(mvp, collisionShape, color);
}
#endif

pDynamicObject::~pDynamicObject()
{
}
