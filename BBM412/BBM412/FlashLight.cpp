#include "FlashLight.h"

FlashLight::FlashLight(const string & obj, const glm::vec3 & dis):
	Moveable(dis),DefaultDynamicObject{obj,dis},GSpotLight{obj,obj+"_light",dis}
{
#ifdef _DEBUG
	GObject::throwError("FlashLight()[enter]:\n");
#endif
	toggleScale = glm::scale( glm::vec3{1.0f } );
	toggleTranslation = glm::translate(glm::vec3{ 1 ,-1,-4.5 });
	toggleRotation = glm::mat4{ 1.0f };
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

		GDynamicDefaultObject::model = inverseCamera * toggleModel;
		GSpotLight::model = inverseCamera * toggleModel;

		Moveable::reset(inverseCamera*toggleTranslation, inverseCamera*toggleRotation);

	}
	else
	{
		onHand = true;
		Moveable::position = glm::vec3(toggleTranslation * glm::vec4(0, 0, 0, 1));
		spotlight.position = glm::vec3(translation * glm::vec4(0, 0, 0, 1));
		spotlight.direction = glm::vec3{ 0,0,-1 };
	}
}

inline void FlashLight::toggleDefaultRender()
{
	
}

void FlashLight::render()
{
	if (onHand)
	{
		GSpotLight::toggleRender(toggleModel);
		DefaultDynamicObject::toggleRender(toggleModel);
	}
	else
	{
		GSpotLight::render();
		DefaultDynamicObject::render();
	}
}

bool FlashLight::isOnHand()
{
	return onHand;
}

void FlashLight::calculateLightening()
{
	if (!onHand)
	{
		SpotLight::calculateLightening();
	}

}

void FlashLight::translate(glm::vec3 displacement)
{
}

void FlashLight::translate(float x, float y, float z)
{
}

void FlashLight::translateOn(float x, float y, float z, const Directions & _dir)
{
}

void FlashLight::rotate(float x, float y, float z)
{
}

void FlashLight::rotateAround(float x, float y, float z, const Directions & dir)
{
}

FlashLight::~FlashLight()
{
}

