#pragma once
#include <glew.h>
#include <glfw\glfw3.h>
#include <glm\ext.hpp>

#include <algorithm>

#include "GameObject.h"


#include "GBasicTextureObject.h"

#include "Sun.h"
#include "Seon.h"
#include "FlashLight.h"

#include "Camera.h"
#include "pPerson.h"
#include "GExceptions.h"

#include "DefaultDynamicObject.h"
#include "DefaultStaticObject.h"

#include "Bullet\include\btBulletDynamicsCommon.h"

#ifdef _DEBUG
	#include "OpenGL\include\gDebug\GLDebugDrawer.h"
#endif

#include "Bullet\include\LinearMath\btVector3.h"

#include "stddef.hpp"

enum BUTTON_STATES {
	RELEASE,	
	PRESS,		
	TOGGLE,
	RELEASED
};
enum { BLUR_HOR, BLUR_VER, HDR_0, HDR_1, FINAL, POST_SIZE };

class World
{
	Tools tool;
	Options option;

	pPerson * player;

	shared_ptr< btDiscreteDynamicsWorld > dynamicsWorld;

	btSequentialImpulseConstraintSolver* solver;
	btCollisionDispatcher* dispatcher;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btBroadphaseInterface* broadphase;

	vector< shared_ptr< GObject > > objects;

	shared_ptr<Sun> sun;
	shared_ptr<FlashLight> flashLight;

	vector< shared_ptr<Light> > lights;

	vector< shared_ptr<PointLight> > point_lights;
	vector< shared_ptr<DirecLight> > direc_lights;
	vector< shared_ptr<SpotLight> > spot_lights;

	shared_ptr<Moveable> picked_obj;
	vector< shared_ptr<Moveable> > toggled_objs;

	vector< shared_ptr<DefaultDynamicObject> > dynamic_objects;
	vector< shared_ptr<DefaultStaticObject> > static_objects;

	BUTTON_STATES buttonStateLeft{ RELEASE }, buttonStateMiddle{ RELEASE }, buttonStateRight{ RELEASE };

	
	inline void resetTools(Tools last, Tools current);
	inline void toggle();
	inline void pick();
	inline void calculateButtonsStates(double deltaTime);
	
	inline void calculateLights(double deltaTime);

	inline void initializeAll();
	inline void createObjects();
	inline GLfloat heightOf( GLfloat x, GLfloat z );



	const unsigned int SCREEN_WIDTH, SCREEN_HEIGHT;

	inline void shadowRender();
	inline void shadowRender( Light * light );
	inline void allRender();

	inline void stepSimulation(const double & deltaTime);
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
	bool debugDrawOn{ true };
	GLDebugDrawer *gDebug;
	void loopDebugView();
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


};

