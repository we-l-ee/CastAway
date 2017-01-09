#pragma once
#include "SpotLight.h"
#include "GBasicLightObject.h"

class GSpotLight :
	public SpotLight, public GBasicLightObject
{
public:
	GSpotLight(const std::string & obj, const GLuint & _prog = (GLuint)RenderMode::WIREFRAME );
	GSpotLight(const std::string & obj, const glm::vec3 & dis, const GLuint & _prog = (GLuint)RenderMode::WIREFRAME );
	
	GSpotLight(const std::string & sub, const std::string & obj, const GLuint & _prog = (GLuint)RenderMode::WIREFRAME );
	GSpotLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, const GLuint & _prog = (GLuint)RenderMode::WIREFRAME );
	GSpotLight(const std::string & sub, const std::string & obj, const glm::vec3 & dis, const GLuint & _vbo, const GLuint & _points_size,
		const GLuint & _prog = (GLuint)RenderMode::WIREFRAME ) ;

	GSpotLight(const std::string & obj, const GLuint & _vbo, const GLuint & _points_size, const GLuint & _prog = (GLuint)RenderMode::WIREFRAME);
	GSpotLight(const std::string & obj, const glm::vec3 & dis, const GLuint & _vbo, const GLuint & _points_size, 
		const GLuint & _prog = (GLuint)RenderMode::WIREFRAME);
	GSpotLight(const std::string & sub, const std::string & obj, const GLuint & _vbo, const GLuint & _points_size, 
		const GLuint & _prog = (GLuint)RenderMode::WIREFRAME );



	// Inherited via Moveable
	virtual void translate(glm::vec3 displacement) override;

	virtual void translate(float x, float y, float z) override;
	virtual void translateOn(float x, float y, float z, const Directions & _dir) override;

	virtual void rotate(float x, float y, float z) override;
	virtual void rotateAround(float x, float y, float z, const Directions & dir) override;

	~GSpotLight();
};

