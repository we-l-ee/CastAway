#include "MapleTree.h"


GLuint MapleTree::VBO, MapleTree::VAO;
unsigned int MapleTree::POINTS_SIZE;
vector<Texture> MapleTree::TEXTURES;
vector<unsigned int> MapleTree::SWITCH_POINTS;
btCollisionShape * MapleTree::shape;


MapleTree::MapleTree(const glm::vec3 & displacement):MapleTree(displacement, glm::vec3{1.0f})
{
}

MapleTree::MapleTree(const glm::vec3 & displacement, const glm::vec3 & scale):GObject(glm::translate(displacement)*glm::scale(scale)),
GDefaultMultiTextureObject(VBO,VAO,POINTS_SIZE,TEXTURES,SWITCH_POINTS,displacement)
{
	addRigidBody("objects\\maple\\maple.rigid", displacement, shape);
}

void MapleTree::initialize()
{
	GDefaultMultiTextureObject::constructer("maple", "maple", TEXTURES, SWITCH_POINTS, VAO, VBO, POINTS_SIZE);
	shape = readCollisionFile("objects\\maple\\maple.collision");

}


MapleTree::~MapleTree()
{
}
