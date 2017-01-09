#pragma once
#include "GObject.h"
#include "Moveable.h"
#include "Camera.h"
#include "DirecLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "glm\ext.hpp"

class GBasicTextureObject:
	public GObject
{


protected:

	static GLuint program;
	static GLuint uniform_index[3];

	GLuint VAO, VBO;

	Texture texture;
	Texture defaultTexture;


	unsigned int points_size;

	//vector<glm::vec3> points;
	//vector<glm::vec2> text_cords;
	//vector<glm::vec3> normals;

	glm::mat4 model;

	GLuint current_program;
protected:

	inline void initTexture(string path);
	
	inline void defaultRender();
	inline void wireframeRender();
	inline void shadowCalculationRender();

	inline void defaultToggleRender(const glm::mat4 & model);
	inline void wireframeToggleRender(const glm::mat4 & model);
	inline void shadowCalculationToggleRender(const glm::mat4 & model);


public:

	void setTexture(Texture tex);
	void setDefaultTexture();


	GBasicTextureObject(const string & obj, const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::BASIC_TEXTURE]);
	GBasicTextureObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::BASIC_TEXTURE]);
	GBasicTextureObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::BASIC_TEXTURE]);
	GBasicTextureObject(const string & sub,const string & obj, const glm::mat4 & _model, const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::BASIC_TEXTURE]);


	GBasicTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::BASIC_TEXTURE]);
	GBasicTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const glm::mat4 & _model, const GLuint & _prog = GObject::GProgram[(GLuint)RenderMode::BASIC_TEXTURE]);


	virtual void render() override;
	virtual void toggleRender(const glm::mat4 & model) override;


	~GBasicTextureObject();

#ifdef _DEBUG
	virtual void debugRender() override;
#endif
};

