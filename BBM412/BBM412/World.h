#pragma once
#include <glew.h>
#include <glfw\glfw3.h>
#include <glm\ext.hpp>

#include <algorithm>

#include "ShadowMapper.h"

#include "GameObject.h"

#include "GMapAObject.h"
#include "GMapRObject.h"
#include "GBasicTextureObject.h"
#include "GBasicTextureLightObject.h"
#include "GDefaultMultiTextureObject.h"

#include "Sun.h"
#include "Moon.h"
#include "Seon.h"
#include "FlashLight.h"
#include "Fern.h"
#include "MapleTree.h"
#include "PineTree.h"
#include "Log.h"
#include "GreyRock.h"
#include "RedRock.h"

#include "Sky.h"

#include "Camera.h"
#include "pPerson.h"
#include "GExceptions.h"

#include "DefaultDynamicObject.h"
#include "DefaultStaticObject.h"
#include "StaticDefaultSplatObject.h"


#include "Bullet\include\btBulletDynamicsCommon.h"

#ifdef _DEBUG
	#include "OpenGL\include\gDebug\GLDebugDrawer.h"
#endif

#include "Bullet\include\LinearMath\btVector3.h"

#include "stddef.hpp"
enum BUTTONS {
	KEY_W, KEY_A, KEY_S, KEY_D, 
	KEY_Q, KEY,E
};

enum BUTTON_STATES {
	RELEASE,	
	PRESS,		
	TOGGLE,
	RELEASED
};
enum { BLUR_HOR, BLUR_VER, HDR_0, HDR_1, FINAL, POST_SIZE };

class World
{
	Tools tool{ Tools::Pick };
	Options option;

	pPerson * player;

	shared_ptr< btDiscreteDynamicsWorld > dynamicsWorld;

	btSequentialImpulseConstraintSolver* solver;
	btCollisionDispatcher* dispatcher;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btBroadphaseInterface* broadphase;


	shared_ptr<Sun> sun;
	shared_ptr<FlashLight> flashLight;

	vector< shared_ptr<Light> > lights;

	vector< shared_ptr<PointLight> > point_lights;
	vector< shared_ptr<DirecLight> > direc_lights;
	vector< shared_ptr<SpotLight> > spot_lights;

	Moveable* pickedRigidBody;
	Light* pickedLight;
	vector< Moveable* > toggled_objs;

	vector< shared_ptr< GObject > > objects;

	vector< shared_ptr<GObject> > scene_objects;
	vector< shared_ptr<GObject> > shadow_rendered_objects;
	vector< pair<bool, shared_ptr<GObject> > > maybe_rendered;

	vector< shared_ptr<ShadowMapper> > shadow_casters;
	//vector< shared_ptr<ReflectionMapper> > reflection_objects;

	vector< shared_ptr<DefaultDynamicObject> > dynamic_objects;
	vector< shared_ptr<DefaultStaticObject> > static_objects;

	BUTTON_STATES buttonStateLeft{ RELEASE }, buttonStateMiddle{ RELEASE }, buttonStateRight{ RELEASE };

	
	inline void resetTools(Tools last, Tools current);

	inline void toggle();
	inline void seon();
	inline void pick();
	inline void lightPick();

	inline void calculateButtonsStates(double deltaTime);
	
	inline void calculateLights(double deltaTime);
	//For packing initializing and creating objects
	//I would much rather put this processing depending outside file. Would be much neat.(LATER)
	inline void initializeAll();
	inline void createObjects();
	inline GLfloat heightOf( GLfloat x, GLfloat z );



	const unsigned int SCREEN_WIDTH, SCREEN_HEIGHT;

	inline void shadowRender();
	inline void shadowRender( Light * light, unsigned int unit);
	GLuint reflectionFBO, reflectionTexture;
	inline void reflectionRender();
	shared_ptr<GMapRObject> ocean;


	inline void allRender();

	inline void stepSimulation(const double & deltaTime);

	RenderMode renderMode{ RenderMode::DEFAULT };

	template<typename T>
	void deleteOccurence(vector< shared_ptr<T> > list, T item);
public:

	World() :World(1600, 800) {}
	World(unsigned int screen_width, unsigned int screen_height);


	void render(const double & deltaTime);


	void processMouseButtons(int state_left, int state_middle, int state_right);
	void processMouseOffsets(double xOffset, double yOffset);
	void processScrollOffsets(double xOffset, double yOffset);
	void processKeys( const bool key[], const double & deltaTime);

	~World();

#ifdef _DEBUG
	GLuint debug_program;
	bool debugDrawOn{ true };
	bool debugRenderOn{ false };
	unsigned int debugView{ 0 };
	void calculateDebugView();
	void loopDebugView();
	GLDebugDrawer *gDebug;
	void debugRenderScene(GLuint texture);
#endif



private:
	//Post processings.
	GLuint fbo[POST_SIZE];
	GLuint texture[POST_SIZE];

	GLuint vbo[POST_SIZE], vao[POST_SIZE], program[POST_SIZE];

	inline void initializePostProcessings();

	inline void blurScene();
	inline void finalRender();
	inline void renderScene();


	inline void bloomRender();

};

template<typename T>
inline void World::deleteOccurence(vector<shared_ptr<T>> list, T item)
{
}
