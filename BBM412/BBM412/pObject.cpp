#include "pObject.h"

std::map<const btCollisionObject*, pObject*> pObject::list;

shared_ptr< btDiscreteDynamicsWorld > pObject::world;



pObject::pObject()
{
}

void pObject::addRigidBody(const string & obj, const glm::vec3 & dis)
{
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(dis.x, dis.y, dis.z)));

	btRigidBody *rigidBody = readObjectFile(obj, motionState);

	pObject::world->addRigidBody(rigidBody);

	list.insert(std::pair<btCollisionObject*, pObject*>((btCollisionObject*)rigidBody, this));
}

inline btBvhTriangleMeshShape* pObject::readTrimesh(ifstream & in)
{
#if (DEBUG_LVL>=2)
	int i = 0;
#endif

	btTriangleMesh *triMesh = new btTriangleMesh();

	vector< btVector3 > v;
	string cmd;
	in >> cmd;
	while (!in.eof()) {
		GLfloat p[3]; GLuint pi[3];

#if (DEBUG_LVL>=2)
		cerr << "in while -> cmd:" << cmd << endl;
#endif

		if (cmd == "v") {
			in >> p[0] >> p[1] >> p[2];
			btVector3 vec{ p[0],p[1],p[2] };
			v.push_back(vec);

#if (DEBUG_LVL>=2)
			cerr << "v:" << endl;
			cerr << p[0] << "," << p[1] << "," << p[2] << endl;
#endif

		}
		else if (cmd == "f") {
			in >> pi[0] >> pi[1] >> pi[2];				

#if (DEBUG_LVL>=2)
			cerr << "f:" << endl;
			cerr << pi[0] << "," << pi[1] << "," << pi[2] << endl
				<< pi[3] << "," << pi[4] << "," << pi[5] << endl
				<< pi[6] << "," << pi[7] << "," << pi[8] << endl;

			cerr << "a:" << a << endl;
#endif
			triMesh->addTriangle(
				v[pi[0]],
				v[pi[1]],
				v[pi[2]]
			);



#if (DEBUG_LVL>=2)
			cout << points[i].x << "," << points[i].y << "," << points[i].z << endl;
			cout << points[i + 1].x << "," << points[i + 1].y << "," << points[i + 1].z << endl;
			cout << points[i + 2].x << "," << points[i + 2].y << "," << points[i + 2].z << endl;
			i += 3;
#endif


		}
		in.ignore(1024, '\n');

		in >> cmd;
	}
	return 	new btBvhTriangleMeshShape(triMesh, true);
}

btRigidBody *  pObject::readObjectFile(const string & path, btMotionState * motionState)
{
#if (DEBUG_LVL>=2)
	int i = 0;
#endif
	GLfloat p[3];

	ifstream in{ path };
	if (!in.is_open())
		throw GException("File can not open for creating rigidBody in pObject::readObjectFile(). Path:\n"+path);

	btScalar mass = 0;
	btVector3 localinertia(0, 0, 0);

	string cmd;
	in >> cmd;
	if (cmd == "mass")
	{
		in >> mass;
		in.ignore(1024, '\n');
		in >> cmd;
	}
	if (cmd == "localinertia")
	{
		in >> p[0] >> p[1] >> p[2];
		localinertia = btVector3(p[0], p[1], p[2]);
		in.ignore(1024, '\n');
		in >> cmd;

	}
	in.ignore(1024, '\n');
	if (cmd == string("trimesh") )
	{
		btCollisionShape *shape = (btCollisionShape*) readTrimesh(in);
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			mass,                  // mass
			motionState,        // initial position
			shape,              // collision shape of body
			localinertia  // local inertia
		);
		return new btRigidBody(rigidBodyCI);
	}
	else if (cmd == "box")
	{
		in >> p[0] >> p[1] >> p[2];
		btCollisionShape *shape = (btCollisionShape*) new btBoxShape( btVector3(p[0], p[1], p[2]) );
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			mass,                  // mass
			motionState,        // initial position
			shape,              // collision shape of body
			localinertia  // local inertia
		);
		return new btRigidBody(rigidBodyCI);
	}

	throw GException("pObject could not constructed\n");
}





pObject * pObject::getpObject(const btCollisionObject * obj)
{
	return list[obj];
}

void pObject::initialize(shared_ptr < btDiscreteDynamicsWorld  > _world)
{
	world = _world;
}

glm::mat4 pObject::btTransform_glm(btTransform * transform)
{
	GLfloat data[16];
	transform->getOpenGLMatrix(data);
	return glm::make_mat4(data);
}

btTransform pObject::glm_btTransform(glm::mat4 matrix)
{
	GLfloat *data = glm::value_ptr(matrix);
	btTransform transform;
	transform.setFromOpenGLMatrix(data);
	return transform;
}

glm::vec3 pObject::btVector3_glm(const btVector3 &vector)
{
	return glm::vec3(vector.getX(), vector.getY(), vector.getZ());
}

btVector3 pObject::glm_btVector3(glm::vec3 vector)
{
	return btVector3(vector.x, vector.y, vector.z);

}


pObject::~pObject()
{
}