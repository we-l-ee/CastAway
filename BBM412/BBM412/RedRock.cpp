#include "RedRock.h"


GLuint RedRock::VBO, RedRock::VAO;
unsigned int RedRock::POINTS_SIZE;
vector<Texture> RedRock::TEXTURES;
vector<unsigned int> RedRock::SWITCH_POINTS;
btCollisionShape * RedRock::shape;


inline void RedRock::collisionCheck()
{
}

RedRock::RedRock(const glm::vec3 & displacement) :
	GObject(glm::translate(displacement)), Moveable(displacement),
	GDefaultMultiTextureObject(VBO, VAO, POINTS_SIZE, TEXTURES, SWITCH_POINTS, displacement)
{
	rigidBody = addRigidBody("objects\\rock3\\rock3.rigid", displacement, shape);
	ALPHA = false;

}


void RedRock::initialize()
{
	GDefaultMultiTextureObject::constructer("rock3", "rock3", TEXTURES, SWITCH_POINTS, VAO, VBO, POINTS_SIZE);
	shape = readCollisionFile("objects\\rock3\\rock3.collision");

}



void RedRock::translate(glm::vec3 displacement)
{
	Moveable::translate(displacement);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));
}

void RedRock::translate(float x, float y, float z)
{
	Moveable::translate(x, y, z);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));

}

void RedRock::translateOn(float x, float y, float z, const Directions & _dir)
{
	Moveable::translateOn(x, y, z, _dir);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));
}

void RedRock::rotate(float x, float y, float z)
{
	Moveable::rotate(x, y, z);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));

}

void RedRock::rotateAround(float x, float y, float z, const Directions & dir)
{
	Moveable::rotateAround(x, y, z, dir);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));

}


RedRock::~RedRock()
{
}
