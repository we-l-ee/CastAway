#pragma once
#include "PointLight.h"
#include "GBasicLightObject.h"

class GPointLight :
	public PointLight, public GBasicLightObject
{
public:

	GPointLight(const PointLightData & _pointlight, 
		const GLuint & _vbo, const GLuint & _points_size, const glm::vec4 & _color, 
		const glm::vec3 & dis = glm::vec3(.0f), const GLuint & _prog = (GLuint)RenderMode::WIREFRAME );

	GPointLight(const std::string & obj, const GLuint & _prog = (GLuint)RenderMode::WIREFRAME );
	GPointLight(const std::string & sub, const std::string & obj, const GLuint & _prog = (GLuint)RenderMode::WIREFRAME );
	GPointLight(const std::string & obj, const glm::vec3 & dis, const GLuint & _prog = (GLuint)RenderMode::WIREFRAME );
	GPointLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, const GLuint & _prog = (GLuint)RenderMode::WIREFRAME );

	GPointLight(const std::string & obj, const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog = (GLuint)RenderMode::WIREFRAME);
	GPointLight(const std::string & sub, const std::string & obj, const GLuint & _vbo, const GLuint & _points_size, 
		const GLuint & _prog = (GLuint)RenderMode::WIREFRAME );
	GPointLight(const std::string & obj, const glm::vec3 & dis, const GLuint & _vbo, const GLuint & _points_size, 
		const GLuint & _prog = (GLuint)RenderMode::WIREFRAME);
	GPointLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, const GLuint & _vbo, 
		const GLuint & _points_size, const GLuint & _prog = (GLuint)RenderMode::WIREFRAME );



	// Inherited via Moveable
	virtual void translate(glm::vec3 displacement) override;

	virtual void translate(float x, float y, float z) override;
	virtual void translateOn(float x, float y, float z, const Directions & _dir) override;

	virtual void rotate(float x, float y, float z) override;
	virtual void rotateAround(float x, float y, float z, const Directions & dir) override;


	~GPointLight();


};

