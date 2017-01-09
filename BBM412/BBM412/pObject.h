#pragma once
#include "GObject.h"
#include "GExceptions.h"

#include "Bullet\include\btBulletCollisionCommon.h"
#include "Bullet\include\btBulletDynamicsCommon.h"
#include "Bullet\include\LinearMath\btVector3.h"
#include "stddef.hpp"

#include <map>
#include <fstream>



class pObject
{


protected:
	static shared_ptr< btDiscreteDynamicsWorld > world;
	static  std::map<const btCollisionObject*, pObject*> list;

	virtual void addRigidBodyToDynamicsWorld(btCollisionShape * _shape){}
	virtual void addRigidBody( const string & obj, const glm::vec3 & dis );

	static inline btBvhTriangleMeshShape* readTrimesh(ifstream & in);

public:
	pObject();

	static btRigidBody * readObjectFile(const string & path, btMotionState * motionState);

	static pObject* getpObject(const btCollisionObject * obj);
	static void initialize(shared_ptr< btDiscreteDynamicsWorld > world);

	static glm::mat4 btTransform_glm(btTransform * transform);
	static btTransform glm_btTransform(glm::mat4 matrix);
	static glm::vec3 btVector3_glm(const btVector3 &vector);
	static btVector3 glm_btVector3(glm::vec3 vector);

	~pObject();
};

