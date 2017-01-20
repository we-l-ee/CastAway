#pragma once
#include "GDefaultMultiTextureObject.h"
#include "pObject.h"

class MapleTree :
	public GDefaultMultiTextureObject, public pObject
{
	static GLuint VBO, VAO;
	static unsigned int POINTS_SIZE;
	static vector<Texture> TEXTURES;
	static vector<unsigned int> SWITCH_POINTS;
	static btCollisionShape * shape;

public:
	MapleTree(const glm::vec3 & displacement );
	
	MapleTree(const glm::vec3 & displacement, const glm::vec3 & scale);

	static void initialize();

	~MapleTree();
};

