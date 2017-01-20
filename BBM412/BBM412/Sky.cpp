#include "Sky.h"


GLuint Sky::VAO, Sky::VBO;
unsigned int Sky::POINTS_SIZE;
Texture Sky::DAY, Sky::NIGHT;

Sky::Sky(const glm::vec3 & displacement) : GTiledTextureObject(VBO, VAO, POINTS_SIZE, DAY), GObject(glm::translate(displacement))
{
}

void Sky::render()
{
	GModel = glm::translate(camera->getPosition());
	if(isDay()){	setTexture(DAY); setTiling(1.0f);	}
	else { setTexture(NIGHT); setTiling(4.0f); }

	GTiledTextureObject::render();
}

void Sky::initialize()
{
	NIGHT = createTexture("texture\\sky\\night.dds");
	GTiledTextureObject::construct("sky", "sky", VBO, VAO, POINTS_SIZE, DAY);
}

Sky::~Sky()
{
}
