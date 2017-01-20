#include "Moon.h"
GLuint Moon::VAO, Moon::VBO;
unsigned int Moon::POINTS_SIZE;
DirecLightData Moon::DIRECLIGHT_DATA{ glm::vec3{0,0,0}, glm::vec3{1,1,1} };
Texture Moon::TEXTURE;



Moon::Moon() :DirecLight(DIRECLIGHT_DATA, glm::vec3{ 200,900,100 }),GObject(glm::translate(glm::vec3{ 200,900,100 })),
GBasicTextureObject(VBO, VAO, POINTS_SIZE, TEXTURE, glm::vec3{ 200,900,100 }), GBasicLightObject( VAO, VBO, POINTS_SIZE, glm::vec4{ 1,1,1,0.2 } )
{

	modelMoon = GModel;
	modelLight = GModel*glm::scale(glm::vec3{ 1.05f });

	toggleActive();
}

void Moon::render()
{

	if (isActive())
	{
		glm::mat4 trans = glm::translate(camera->getPosition());
		GModel = trans*modelMoon;
		GBasicTextureObject::render();
		GModel = trans*modelLight;
		GBasicLightObject::render();
	}
}

void Moon::toggleRender(const glm::mat4 & model)
{
	if (isActive())
	{
		GBasicLightObject::toggleRender(model);
		GBasicTextureObject::toggleRender(model);
	}
}

void Moon::calculateLightening()
{
	if (degreeOfDay < 180 && isActive())
		toggleActive();
	else if (degreeOfDay >= 180 && !isActive())
		toggleActive();

}

void Moon::initialize()
{
	GBasicTextureObject::construct("moon", "moon", VBO, VAO, POINTS_SIZE, TEXTURE);
	DIRECLIGHT_DATA = readLightFile("objects\\moon\\moon.light");
}
#ifdef _DEBUG
void Moon::debugRender()
{
	if (isActive())
	{
		GBasicLightObject::debugRender();
		GBasicTextureObject::debugRender();
	}
}
#endif

Moon::~Moon()
{
}
