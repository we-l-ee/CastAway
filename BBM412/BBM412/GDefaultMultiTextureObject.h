#pragma once
#include "GObject.h"
#include "Moveable.h"
#include "Camera.h"
#include "DirecLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "glm\ext.hpp"

class GDefaultMultiTextureObject :
	public virtual GObject
{


protected:
	static GLuint program;

	GLuint vao, vbo;

	glm::vec3 m_specular;
	GLfloat shininess;

	vector<Texture> defaultTextures;
	vector<Texture> textures;
	vector<unsigned int> switch_points;

	vector<glm::vec3> points;
	vector<glm::vec2> text_cords;
	vector<glm::vec3> normals;

	unsigned int points_size;

	GLuint current_program;

	bool ALPHA{ true };

protected:

	inline void defaultRender();
	inline void basicTextureRender();
	inline void wireframeRender();
	inline void shadowCalculationRender();
	inline void reflectionCalculationRender();

	inline void defaultToggleRender(const glm::mat4 & model);
	inline void basicTextureToggleRender(const glm::mat4 & model);
	inline void wireframeToggleRender(const glm::mat4 & model);
	inline void shadowCalculationToggleRender(const glm::mat4 & model);
	inline void reflectionCalculationToggleRender(const glm::mat4 & model);

public:

	GDefaultMultiTextureObject(const string & obj, const GLuint & _prog = program);
	GDefaultMultiTextureObject(const string & obj, const glm::vec3 & dis, const GLuint & _prog = program);
	GDefaultMultiTextureObject(const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular,
		const GLfloat & shininess, const GLuint & _prog = program);

	GDefaultMultiTextureObject(const string & sub, const string & obj, const GLuint & _prog = program);
	GDefaultMultiTextureObject(const string & sub, const string & obj, const glm::vec3 & dis, const GLuint & _prog = program);
	GDefaultMultiTextureObject(const string & sub, const string & obj, const glm::vec3 & dis, const glm::vec3 & _m_specular,
		const GLfloat & shininess, const GLuint & _prog = program);


	GDefaultMultiTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const vector<Texture> & textures,
		const vector<unsigned int> & switch_points, const GLuint & _prog = program);
	GDefaultMultiTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const vector<Texture> & textures,
	const vector<unsigned int> & switch_points, const glm::vec3 & displacement, const GLuint & _prog = program);

	GDefaultMultiTextureObject(const GLuint & vbo, const GLuint & vao, const unsigned int & _points_size, const vector<Texture> & textures,
		const vector<unsigned int> & switch_points, const glm::vec3 & displacement,
		const glm::vec3 & _m_specular, const GLfloat & shininess, const GLuint & _prog = program);

	virtual void render() override;
	virtual void toggleRender(const glm::mat4 & model) override;

	static void constructer(const string & sub, const string & obj, vector<Texture> & textures,
		vector<unsigned int> & switch_points, GLuint & vao, GLuint & vbo, unsigned int & points_size);
	static vector<Texture> initTextures(const vector<string> & texts);

	~GDefaultMultiTextureObject();
	static void initialize();
#ifdef _DEBUG
	// Inherited via GObject
	virtual void debugRender() override;
#endif // _DEBUG

};

