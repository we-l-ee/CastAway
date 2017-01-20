#include "GreyRock.h"


GLuint GreyRock::VBO, GreyRock::VAO;
unsigned int GreyRock::POINTS_SIZE;
vector<Texture> GreyRock::TEXTURES;
vector<unsigned int> GreyRock::SWITCH_POINTS;
btCollisionShape * GreyRock::shape;


GreyRock::GreyRock(const glm::vec3 & displacement):
GObject(glm::translate(displacement)),Moveable(displacement),
GDefaultMultiTextureObject(VBO, VAO, POINTS_SIZE, TEXTURES, SWITCH_POINTS, displacement)
{
	rigidBody = addRigidBody("objects\\rock2\\rock2.rigid", displacement, shape);
	ALPHA = false;

}


void GreyRock::initialize()
{
	GDefaultMultiTextureObject::constructer("rock2", "rock2", TEXTURES, SWITCH_POINTS, VAO, VBO, POINTS_SIZE);
	shape = readCollisionFile("objects\\rock2\\rock2.collision");

}



void GreyRock::translate(glm::vec3 displacement)
{
	Moveable::translate(displacement);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));
}

void GreyRock::translate(float x, float y, float z)
{
	Moveable::translate(x, y, z);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));

}

void GreyRock::translateOn(float x, float y, float z, const Directions & _dir)
{
	Moveable::translateOn(x, y, z, _dir);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));
}

void GreyRock::rotate(float x, float y, float z)
{
	Moveable::rotate(x, y, z);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));

}

void GreyRock::rotateAround(float x, float y, float z, const Directions & dir)
{
	Moveable::rotateAround(x, y, z, dir);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));

}


GreyRock::~GreyRock()
{
}
