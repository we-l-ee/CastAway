#pragma once
#include "GMapAObject.h"
class Fern : public GMapAObject
{
	static GLuint VAO, VBO, points_size;
	static Texture texture;
	static vector<glm::vec3> points;
	static vector<glm::vec2> text_cords;
	static vector<glm::vec3> normals;

public:
	Fern(const glm::vec3 & displacement, const glm::vec3 & scale);

	static void initialize();

	~Fern();
};

