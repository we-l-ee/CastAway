#include "FlashLight.h"

void FlashLight::toggleNoRender()
{
	noRender = !noRender;
}

FlashLight::FlashLight(const string & obj, const glm::vec3 & dis):
	Moveable(dis),GObject(glm::translate(dis)),DefaultDynamicObject{obj,dis},GSpotLight{obj,obj+"_light",dis}
{
#ifdef _DEBUG
	GObject::throwError("FlashLight()[enter]:\n");
#endif
	toggleScale = glm::scale( glm::vec3{1.0f } );
	toggleTranslation = glm::translate(glm::vec3{ 1 ,-1,-3.0 });
	toggleRotation = glm::mat4{ 1.0 };
		//glm::rotate( glm::radians(180.0f), glm::vec3{ 0,1, 0 } );
	toggleModel = toggleTranslation*  toggleRotation* toggleScale;

#ifdef _DEBUG
	GObject::throwError("FlashLight()[exit]:\n");
#endif
}

void FlashLight::toggleOnHand()
{
	if (onHand)
	{
		onHand = false;
		glm::mat4 inverseCamera = glm::inverse(camera->getViewMatrix());

		GModel = inverseCamera * (toggleTranslation*  toggleRotation);

		pObject::rigidBody->setWorldTransform(pObject::glm_btTransform(GDynamicDefaultObject::GModel));
		Moveable::reset(inverseCamera*toggleTranslation, inverseCamera*toggleRotation);

	}
	else
	{
		onHand = true;
		Moveable::position = glm::vec3(toggleTranslation * glm::vec4(0, 0, 0, 1));
		spotlight.position = glm::vec3(toggleTranslation * glm::vec4(0, 0, 0, 1));
		spotlight.direction = glm::vec3{ 0,0,-1 };
	}
}

void FlashLight::render()
{
	if (noRender)
		return;
	
	if (onHand)
	{
		DefaultDynamicObject::toggleRender(toggleModel);
		if (isActive())
			GSpotLight::toggleRender(toggleModel);
	}
	else
	{
		DefaultDynamicObject::render();
		if (isActive())
			GSpotLight::render();
	}
}

void FlashLight::toggleRender(const glm::mat4 & matrix)
{
	
}

glm::mat4 FlashLight::getLightViewMatrix()
{

	if (onHand)
		return  camera->getViewMatrix();
	else
		return glm::lookAt(position, position + dir.Forward, dir.Up);
}

glm::mat4 FlashLight::getLightProjMatrix()
{
	return glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);
}


bool FlashLight::isOnHand()
{
	return onHand;
}
#ifdef _DEBUG
void FlashLight::debugRender()
{
	if (noRender)
		return;
	DefaultDynamicObject::debugRender();
}
#endif
void FlashLight::calculateLightening()
{
	if (!onHand)
	{
		SpotLight::calculateLightening();
	}

}

void FlashLight::translate(glm::vec3 displacement)
{
	DefaultDynamicObject::translate(displacement);
}

void FlashLight::translate(float x, float y, float z)
{
	DefaultDynamicObject::translate(x,y,z);
}

void FlashLight::translateOn(float x, float y, float z, const Directions & _dir)
{
	DefaultDynamicObject::translateOn(x, y, z, _dir);
}

void FlashLight::rotate(float x, float y, float z)
{
	DefaultDynamicObject::rotate(x, y, z);
}

void FlashLight::rotateAround(float x, float y, float z, const Directions & dir)
{
	DefaultDynamicObject::rotateAround(x, y, z, dir);
}

FlashLight::~FlashLight()
{
}

