#pragma once

#include <glew.h>
#include <string>
#include <memory>
#include <vector>
#include <glm\ext.hpp>
#include <gli\gli.hpp>

#include "GameObject.h"
#include "DirecLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include <iostream>

#include "stddef.hpp"

#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))

using namespace std;

class GObject : public virtual GameObject
{
private:
private:

	static GLuint compileShader(const char * file, GLenum type);

protected:
	glm::vec4 wireframeColor{ 1,0,0,1 };
	static glm::mat4 reflectionMatrix;

	static glm::mat4 lightView;
	static glm::mat4 lightViewProj;

	static glm::mat4 sunView;
	static glm::mat4 sunViewProj;

	static glm::mat4 spotView;
	static glm::mat4 spotViewProj;

#ifdef _DEBUG
	static glm::mat4 debug_view;
	static glm::mat4 debug_viewProj;
#endif
	static GLuint sunShadowMap;

	static GLuint GProgram[(int)RenderMode::SIZE];
	static GLuint VAO[(int)RenderMode::SIZE];

	enum {DIRECTLIGHT,POINTLIGHT,SPOTLIGHT,UNIFORM_BUFFER_SIZE};
	static GLuint uniform_buffers[UNIFORM_BUFFER_SIZE];
	static GLuint nums_uniform_buffers[UNIFORM_BUFFER_SIZE];

	static const vector<shared_ptr<DirecLight>> *direc_lights;
	static const vector<shared_ptr<PointLight>> *point_lights;
	static const vector<shared_ptr<SpotLight>> *spot_lights;

	static unsigned int nextAvaibleTextureUnit;

	static RenderMode renderMode;

	inline void wireframeRender();
	inline void basicTextureRender();

	inline void wireframeToggleRender( const glm::mat4 & model );
	inline void basicTextureToggleRender( const glm::mat4 & model );

	glm::mat4 GModel;

public:

	void setWireframeColor(const glm::vec4 & _color);
	glm::vec4 getWireframeColor();

#ifdef _DEBUG
	virtual void debugRender() = 0;
	static void setDebugViewMatrix(const glm::mat4 & mat);
	static void setDebugViewProjMatrix(const glm::mat4 & view, const glm::mat4 & proj);
#endif

	GObject( const glm::mat4 & _model );

	virtual void render() = 0;
	virtual void toggleRender(const glm::mat4 & model) = 0;

	static GLuint createProgram(const char * vshader, const char* fshader);
	static Texture createTexture(string file);

#ifdef _DEBUG
	static std::string returnErrorString(GLuint err);
	static void throwError(string s);
	static void throwError();
#endif

	static void initialize( const vector<shared_ptr<PointLight>>* p_lights, const vector<shared_ptr<DirecLight>>* d_lights, const vector<shared_ptr<SpotLight>>* s_lights);

	
	static void setLights();

	static void setRenderMode(RenderMode mode);

	static void setLightViewMatrix(const glm::mat4 & view, const glm::mat4 & proj);

	static void setSunViewMatrix(const glm::mat4 & view, const glm::mat4 & proj);
	static void setSpotlightViewMatrix(const glm::mat4 & view, const glm::mat4 & proj);

	static void setReflectionMatrix(const glm::mat4 & _reflectionMatrix);

	static void setAvaibleTextureUnit(const unsigned int & text_unit);
	static unsigned int getAvaibleTextureUnit();

	static void GObject::readObjectFile(string file,
		vector<glm::vec3> &points);

	static void GObject::readObjectFile(string file,
		vector<glm::vec3> &points, vector<glm::vec2> &text_cords);

	static void GObject::readObjectFileVN(string file,
		vector<glm::vec3> &points, vector<glm::vec3> &normals
	);

	static void GObject::readObjectFile(string file, 
		vector<glm::vec3> &points, vector<glm::vec2> &text_cords, vector<glm::vec3> &normals
		);
	static void GObject::readObjectFile(string file,
		vector<glm::vec3> &points, vector<glm::vec2> &text_cords, vector<glm::vec3> &normals, 
		vector<string> & texture_names, vector<unsigned int> & switch_points
	);
	static void GObject::readObjectFile(string file,
		vector<glm::vec3> &points, vector<glm::vec2> &text_cords, vector<glm::vec3> &normals,
		Mesh &faces,
		vector<glm::vec3> &v = vector<glm::vec3>{}, vector<glm::vec2> &vt = vector<glm::vec2>{}, vector<glm::vec3> &vn = vector<glm::vec3>{},
		glm::vec3 displacement = glm::vec3{ 0.0f }, GLfloat scale = { 1.0f });

	virtual ~GObject();
};

