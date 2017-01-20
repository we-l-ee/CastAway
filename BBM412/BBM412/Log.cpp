#include "Log.h"


GLuint Log::VBO, Log::VAO;
unsigned int Log::POINTS_SIZE;
vector<Texture> Log::TEXTURES;
vector<unsigned int> Log::SWITCH_POINTS;
btCollisionShape * Log::shape;


Log::Log(const glm::vec3 & displacement) :Log(displacement, glm::vec3{ 1.0f })
{

}

Log::Log(const glm::vec3 & displacement, const glm::vec3 & scale) : GObject(glm::translate(displacement)*glm::scale(scale)),
GDefaultMultiTextureObject(VBO, VAO, POINTS_SIZE, TEXTURES, SWITCH_POINTS, displacement)
{
	addRigidBody("objects\\Log\\Log.rigid", displacement, shape);
	ALPHA = false;

}

void Log::initialize()
{
	GDefaultMultiTextureObject::constructer("Log", "Log", TEXTURES, SWITCH_POINTS, VAO, VBO, POINTS_SIZE);
	shape = readCollisionFile("objects\\Log\\Log.collision");

}


Log::~Log()
{
}
