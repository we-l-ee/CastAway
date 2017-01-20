#pragma once
#include "GDefaultMultiTextureObject.h"
#include "pObject.h"

class GreyRock:
	public GDefaultMultiTextureObject, public pObject, public Moveable
{
	static GLuint VBO, VAO;
	static unsigned int POINTS_SIZE;
	static vector<Texture> TEXTURES;
	static vector<unsigned int> SWITCH_POINTS;
	static btCollisionShape * shape;

public:
	GreyRock(const glm::vec3 & displacement);

	GreyRock(const glm::vec3 & displacement, const glm::vec3 & scale);

	// Inherited via Moveable
	virtual void translate(glm::vec3 displacement) override;

	virtual void translate(float x, float y, float z) override;
	virtual void translateOn(float x, float y, float z, const Directions & _dir) override;

	virtual void rotate(float x, float y, float z) override;
	virtual void rotateAround(float x, float y, float z, const Directions & dir) override;

	static void initialize();

	~GreyRock();
};

