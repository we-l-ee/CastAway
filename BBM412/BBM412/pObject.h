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

	//virtual void addRigidBodyToDynamicsWorld(btCollisionShape * _shape){}
	virtual btRigidBody * addRigidBody( const string & obj, const glm::vec3 & dis, const glm::quat & rot = glm::quat(0,0,0,1) );
	virtual btRigidBody * addRigidBody( const string & obj, const glm::vec3 & dis, btCollisionShape * _shape, const glm::quat & rot = glm::quat(0, 0, 0, 1) );

	static inline btBvhTriangleMeshShape* readTrimesh(ifstream & in);

	btRigidBody * rigidBody;

public:
	pObject();

	static btRigidBody * readObjectFile(const string & path, btMotionState * motionState);
	static btCollisionShape * readCollisionFile(const string & path);
	static btRigidBody* readRigidFile(const string & path, btMotionState * motionState, btCollisionShape * shape);

	static pObject* getpObject(const btCollisionObject * obj);
	static void initialize(shared_ptr< btDiscreteDynamicsWorld > world);

	static glm::mat4 btTransform_glm(btTransform * transform);
	static btTransform glm_btTransform(const glm::mat4 & matrix);
	static glm::vec3 btVector3_glm(const btVector3 &vector);
	static btVector3 glm_btVector3(glm::vec3 vector);

	~pObject();
};

