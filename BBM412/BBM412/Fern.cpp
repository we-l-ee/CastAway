#include "Fern.h"


GLuint Fern::VAO, Fern::VBO, Fern::points_size;
Texture Fern::texture;
vector<glm::vec3> Fern::points;
vector<glm::vec2> Fern::text_cords;
vector<glm::vec3> Fern::normals;

Fern::Fern(const glm::vec3 & displacement, const glm::vec3 & scale):GObject(glm::translate(displacement)* glm::scale(scale)),
GMapAObject(VBO, VAO, points_size)
{
	setTexture(texture);
}


void Fern::initialize()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	readObjectFile("objects\\fern\\fern.obj", points, text_cords, normals);
	points_size = points.size();

	texture = createTexture("texture\\fern\\fern.dds");

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size() +
		sizeof(glm::vec2)*text_cords.size() + sizeof(glm::vec3)*normals.size(), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3)*points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size(), sizeof(glm::vec2)*text_cords.size(), &text_cords[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*points.size() + sizeof(glm::vec2)*text_cords.size(),
		sizeof(glm::vec3)*normals.size(), &normals[0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(glm::vec3)*points.size()));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(glm::vec3)*points.size() + sizeof(glm::vec2)*text_cords.size()));

	glBindVertexArray(0);
}

Fern::~Fern()
{
}
