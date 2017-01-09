#pragma once
#include <glew.h>
#include <gli\gli.hpp>
#include "Bullet\include\LinearMath\btVector3.h"
#include <vector>


#ifdef _DEBUG
#define DEBUG_LVL 1
#else
#define DEBUG_LVL 0
#endif


#define print(x) std::cout<<#x " "<<x;
#define	newline std::cout<<std::endl;

inline std::ostream& operator<< (std::ostream& stream, const glm::vec3 & vec) {
	stream << "( " << vec.x << ", " << vec.y << ", " << vec.z << " )";
	return stream;
}
inline std::ostream& operator<< (std::ostream& stream, const btVector3 & vec) {
	stream << "( " << vec.getX() << ", " << vec.getY() << ", " << vec.getZ() << " )";
	return stream;
}

inline bool operator== (const glm::vec3 a, const glm::vec3 & b) {

	return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator< (const glm::vec3 a, const glm::vec3 & b) {
	if (a.x == b.x)
	{
		if (a.y == b.y)
		{
			return a.z < b.z;
		}
		else
		{
			if (a.z == b.z)
			{
				return a.y < b.y;
			}
			else
			{
				return a.z < b.z && a.y < b.y;
			}
		}
	}
	else
	{
		if (a.y == b.y)
		{
			if (a.z == b.z)
			{
				return a.x < b.x;
			}
			else
			{
				return a.z < b.z && a.x < b.x;
			}
		}
		else
		{
			if (a.z == b.z)
			{
				return a.x < b.x && a.y < b.y;
			}
			else
			{
				return a.z < b.z && a.x < b.x && a.y < b.y;
			}
		}
	}
	
}
inline bool operator> (const glm::vec3 a, const glm::vec3 & b) {
	return a != b && !(a < b);
}

inline bool operator>= (const glm::vec3 a, const glm::vec3 & b) {
	return !(a<b);
}
inline bool operator<= (const glm::vec3 a, const glm::vec3 & b) {
	return !(a>b);
}

enum class RenderMode
{
	DEFAULT, BASIC_TEXTURE, WIREFRAME, SHADOW_CALC, REFLECTION_CALC, SIZE
};

inline RenderMode operator+(RenderMode a, unsigned int b) {
	return static_cast<RenderMode>( (static_cast<int>(a) + b) % static_cast<int>(RenderMode::SIZE) );
}


enum class Tools {
	Pick, Toggle, Seon, LightPick
};
inline Tools operator&(Tools a, Tools b) {
	return static_cast<Tools>(static_cast<int>(a) | static_cast<int>(b));
}

enum class Options {
	Seon_White, Seon_Red, Seon_Blue, Seon_Random,
	Toggle_Rotate, Toggle_Translate
};

enum class CameraMovement {
	NO_MOVE=0,
	FORWARD=1,
	BACKWARD=1<<1,
	LEFT= 1 << 2,
	RIGHT= 1 << 3,
	UP= 1 << 4,
	DOWN= 1 << 5,
	ROLL_POS= 1 << 6,
	ROLL_NEG= 1 << 7,
	YAW_POS= 1 << 8,
	YAW_NEG= 1 << 9,
	PITCH_POS= 1 << 10,
	PITCH_NEG= 1 << 11
};
inline CameraMovement operator& (CameraMovement a, CameraMovement b) {
	return static_cast<CameraMovement>(static_cast<int>(a) & static_cast<int>(b));
}

inline CameraMovement operator|(CameraMovement a, CameraMovement b) {
	return static_cast<CameraMovement>(static_cast<int>(a) | static_cast<int>(b));
}
inline CameraMovement& operator|=(CameraMovement &a, CameraMovement b) {
	a = static_cast<CameraMovement>(static_cast<int>(a) | static_cast<int>(b));
	return a;
}
enum class CameraMode { DEFAULT, GHOST, FLY };

enum class Speed {
	SLOW,
	NORMAL,
	FAST
};

struct Directions {
	glm::vec3	Forward;
	glm::vec3	Up;
	glm::vec3	Right;
	Directions(glm::vec3 _f = { 0.0,0.0,-1.0 }
		, glm::vec3 _u = { 0.0,1.0,0.0 },
		glm::vec3 _r = { 1.0,0.0,0.0 }) : Forward(_f), Up(_u), Right(_r) {};
};

struct Mesh
{
	std::vector<glm::i16vec3> points;
	std::vector<glm::i16vec3> tex_cords;
	std::vector<glm::i16vec3> normals;

};

struct Texture {
	GLenum target;
	GLuint	tex_name;
};

struct StrengthData {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	glm::vec3 delta_ambient;
	glm::vec3 delta_diffuse;
	glm::vec3 delta_specular;

	StrengthData(
		glm::vec3 _ambient,
		glm::vec3 _diffuse,
		glm::vec3 _specular,
		GLfloat percentage = 0.1f
	) :
		ambient(_ambient),
		diffuse(_diffuse),
		specular(_specular),

		delta_ambient(_ambient * percentage),
		delta_diffuse(_diffuse * percentage),
		delta_specular(_specular * percentage)
	{}


};

struct alignas(sizeof(glm::vec4))DirecLightData {

	alignas(sizeof(glm::vec4))glm::vec3 direction;
	alignas(sizeof(glm::vec4))glm::vec3 color;

	alignas(sizeof(glm::vec4))glm::vec3 ambient;
	alignas(sizeof(glm::vec4))glm::vec3 diffuse;
	alignas(sizeof(glm::vec4))glm::vec3 specular;

	DirecLightData(
		glm::vec3 _direction,
		glm::vec3 _color,

		glm::vec3 _ambient = glm::vec3{ 0.2f, 0.2f, 0.2f },
		glm::vec3 _diffuse = glm::vec3{ 0.4f, 0.4f, 0.4f },
		glm::vec3 _specular = glm::vec3{ 1.0f, 1.0f, 1.0f }

	) :
		direction(_direction),
		color(_color),

		ambient(_ambient),
		diffuse(_diffuse),
		specular(_specular)


	{}
};

struct alignas(sizeof(glm::vec4))PointLightData {

	alignas(sizeof(glm::vec4))glm::vec3 position;
	alignas(sizeof(glm::vec4))glm::vec3 color;

	alignas(sizeof(glm::vec4))glm::vec3 ambient;
	alignas(sizeof(glm::vec4))glm::vec3 diffuse;
	alignas(sizeof(glm::vec4))glm::vec3 specular;

	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

	PointLightData(
		glm::vec3 _position,
		glm::vec3 _color,

		glm::vec3 _ambient = glm::vec3{ 0.2f, 0.2f, 0.2f },
		glm::vec3 _diffuse = glm::vec3{ 0.8f, 0.8f, 0.8f },
		glm::vec3 _specular = glm::vec3{ 1.0f, 1.0f, 1.0f },

		GLfloat _constant = GLfloat{ 1.0f },
		GLfloat	_linear = GLfloat{ 0.09f },
		GLfloat _quadratic = GLfloat{ 0.032f }
	) :
		position(_position),
		color(_color),

		ambient(_ambient),
		diffuse(_diffuse),
		specular(_specular),

		constant(_constant),
		linear(_linear),
		quadratic(_quadratic)
	{}





};

struct alignas(sizeof(glm::vec4))SpotLightData {
	alignas(sizeof(glm::vec4))glm::vec3 position;
	alignas(sizeof(glm::vec4))glm::vec3 direction;
	alignas(sizeof(glm::vec4))glm::vec3 color;

	alignas(sizeof(glm::vec4))glm::vec3 ambient;
	alignas(sizeof(glm::vec4))glm::vec3 diffuse;
	alignas(sizeof(glm::vec4))glm::vec3 specular;

	GLfloat cutOff;
	GLfloat outerCutOff;
	
	GLfloat constant;
	GLfloat	linear;
	GLfloat quadratic;

	SpotLightData(
		glm::vec3 _position,
		glm::vec3 _direction,
		glm::vec3 _color,

		GLfloat _cutOff = { glm::cos(glm::radians(12.5f)) },
		GLfloat _outerCutOff = { glm::cos(glm::radians(15.0f)) },

		glm::vec3 _ambient = glm::vec3{ 0.2f, 0.2f, 0.2f },
		glm::vec3 _diffuse = glm::vec3{ 0.8f, 0.8f, 0.8f },
		glm::vec3 _specular = glm::vec3{ 1.0f, 1.0f, 1.0f },


		GLfloat _constant = GLfloat{ 1.0f },
		GLfloat	_linear = GLfloat{ 0.09f },
		GLfloat _quadratic = GLfloat{ 0.032f }
	) :
		position(_position),
		direction(_direction),
		color(_color),

		ambient(_ambient),
		diffuse(_diffuse),
		specular(_specular),

		cutOff(_cutOff),
		outerCutOff(_outerCutOff),

		constant(_constant),
		linear(_linear),
		quadratic(_quadratic)
	{}

};