#pragma once
#include "GObject.h"
#include "Moveable.h"
#include "Camera.h"
#include "DirecLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "glm\ext.hpp"

class GDefaultSplatObject:
	public GObject
{


protected:

	static GLuint program;
	static GLuint uniform_index[3];

	GLuint VAO, VBO;

	glm::vec3 m_specular;
	GLfloat shininess;

	GLuint buffer;
	Texture Texture1;
	Texture Texture2;
	Texture Texture3;
	Texture Texture4;

	Texture SplatTexture;

	Texture defaultTexture1;
	Texture defaultTexture2;
	Texture defaultTexture3;
	Texture defaultTexture4;

	Texture defaultSplatTexture;


	unsigned int points_size;

	//vector<glm::vec3> points;
	//vector<glm::vec2> text_cords;
	//vector<glm::vec3> normals;

	glm::mat4 model;

protected:

	inline void defaultRender();
	inline void initTexture(string text1, string text2, string text3, string text4, string splat);

public:



	GDefaultSplatObject(const string & obj);
	GDefaultSplatObject(const string & obj, const glm::vec3 & dis);
	GDefaultSplatObject(const string & obj, const glm::mat4 & _model);
	GDefaultSplatObject(const string & obj, const glm::mat4 & _model,
		const glm::vec3 & _m_specular, const GLfloat & shininess);

	GDefaultSplatObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size);
	GDefaultSplatObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const glm::mat4 & _model);
	GDefaultSplatObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size,
		const glm::mat4 & _model, const glm::vec3 & _m_specular, const GLfloat & shininess);

	virtual void render() override;


	static void initialize();

	~GDefaultSplatObject();
};

