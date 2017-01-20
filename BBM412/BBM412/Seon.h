#pragma once
#include "GPointLight.h"
#include "pObject.h"

class Seon:
	public GPointLight, public pObject , public virtual Moveable
{
	static vector<glm::vec3> points;
	static btCollisionShape * shape;
	static GLuint VAO, VBO, points_size;
	static PointLightData POINTLIGHT;

public:

	Seon(const glm::vec3 & _dis, const glm::vec3 & _color );

	static void initialize();

	// Inherited via Moveable
	virtual void translate(glm::vec3 displacement) override;

	virtual void translate(float x, float y, float z) override;
	virtual void translateOn(float x, float y, float z, const Directions & _dir) override;

	virtual void rotate(float x, float y, float z) override;
	virtual void rotateAround(float x, float y, float z, const Directions & dir) override;

	~Seon();
};

