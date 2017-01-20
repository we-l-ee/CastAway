#include "PineTree.h"


GLuint PineTree::VBO, PineTree::VAO;
unsigned int PineTree::POINTS_SIZE;
vector<Texture> PineTree::TEXTURES;
vector<unsigned int> PineTree::SWITCH_POINTS;
btCollisionShape * PineTree::shape;


PineTree::PineTree(const glm::vec3 & displacement) :PineTree(displacement, glm::vec3{ 1.0f })
{
}

PineTree::PineTree(const glm::vec3 & displacement, const glm::vec3 & scale) : GObject(glm::translate(displacement)*glm::scale(scale)),
GDefaultMultiTextureObject(VBO, VAO, POINTS_SIZE, TEXTURES, SWITCH_POINTS, displacement)
{
	addRigidBody("objects\\pine\\pine.rigid", displacement, shape);
}

void PineTree::initialize()
{
	GDefaultMultiTextureObject::constructer("pine", "pine", TEXTURES, SWITCH_POINTS, VAO, VBO, POINTS_SIZE);
	shape = readCollisionFile("objects\\pine\\pine.collision");

}


PineTree::~PineTree()
{
}
