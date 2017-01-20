#pragma once
#include "GObject.h"
#include "Moveable.h"
#include "Camera.h"
#include "DirecLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "glm\ext.hpp"

class GTiledTextureObject :
	virtual public GObject
{


protected:
	static GLuint program;

	GLuint vao, vbo;
	GLfloat tile;

	Texture texture;
	Texture defaultTexture;


	unsigned int points_size;

	GLuint current_program;
protected:

	static void initTexture(const string & path, Texture & texture);

	inline void defaultRender();
	inline void wireframeRender();
	inline void shadowCalculationRender();
	inline void reflectionCalculationRender();

	inline void defaultToggleRender(const glm::mat4 & model);
	inline void wireframeToggleRender(const glm::mat4 & model);
	inline void shadowCalculationToggleRender(const glm::mat4 & model);
	inline void reflectionCalculationToggleRender();


public:
	void setTiling(const GLfloat & tile);

	void setTexture(Texture tex);
	void setDefaultTexture();


	GTiledTextureObject(const string & obj, const GLuint & _prog = program);
	GTiledTextureObject(const string & obj, const glm::vec3 & dis, 
		const GLuint & _prog = program );

	GTiledTextureObject(const string & sub, const string & obj, const glm::vec3 & dis, 
		const GLuint & _prog = program);
	GTiledTextureObject(const string & sub, const string & obj, const glm::mat4 & _model, 
		const GLuint & _prog = program);


	GTiledTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const Texture & texture,
		const GLuint & _prog = program);
	GTiledTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const Texture & texture,
		const glm::vec3 & displacement, const GLuint & _prog = program);

	static void construct(const string & sub, const string & obj, GLuint & vbo, GLuint & vao, unsigned int & points_size, Texture & texture );

	virtual void render() override;
	virtual void toggleRender(const glm::mat4 & model) override;

	static void intialize();

	~GTiledTextureObject();

#ifdef _DEBUG
	virtual void debugRender() override;
#endif
};

