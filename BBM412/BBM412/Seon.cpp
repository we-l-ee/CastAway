#include "Seon.h"
#include <sstream>

GLuint Seon::VAO, Seon::VBO, Seon::points_size;
btCollisionShape * Seon::shape;

vector<glm::vec3> Seon::points;
PointLightData Seon::POINTLIGHT(glm::vec3{ 0,0,0 }, glm::vec3{ 1,1,1 });

Seon::Seon(const glm::vec3 & dis, const glm::vec3 &_color):
	GObject(glm::translate(dis)), GPointLight(POINTLIGHT,VAO, VBO, points_size, glm::vec4( _color,glm::vec1(1.0) ), dis ),Moveable(dis)
{
	pointlight.color = _color;
	rigidBody = addRigidBody("objects\\seon\\seon.rigid", dis, shape);

}

void Seon::initialize()
{

	GObject::readObjectFile("objects\\seon\\seon.obj", points);

	POINTLIGHT = readLightFile("objects\\seon\\seon.light");

	shape = readCollisionFile("objects\\seon\\seon.collision");

	points_size = points.size();
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points_size, &points[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

}


void Seon::translate(glm::vec3 displacement)
{
	Moveable::translate(displacement);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));
}

void Seon::translate(float x, float y, float z)
{
	Moveable::translate(x, y, z);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));

}

void Seon::translateOn(float x, float y, float z, const Directions & _dir)
{
	cout << "Translate" << endl;

	Moveable::translateOn(x, y, z, _dir);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));
}

void Seon::rotate(float x, float y, float z)
{
	Moveable::rotate(x, y, z);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));

}

void Seon::rotateAround(float x, float y, float z, const Directions & dir)
{
	Moveable::rotateAround(x, y, z, dir);
	GModel = calculateModelMatrix();
	rigidBody->setWorldTransform(pObject::glm_btTransform(GModel));

}

Seon::~Seon()
{
}

