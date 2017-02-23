#include "World.h"
#include <iostream>
#include <bitset>



World::World(unsigned int screen_width, unsigned int screen_height) :
	SCREEN_WIDTH(screen_width), SCREEN_HEIGHT(screen_height),
	player(new pPerson{ dynamicsWorld, glm::vec3{ 50,50,50 } })
{
	glGenFramebuffers(1, &reflectionFBO);
	glGenTextures(1, &reflectionTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);


		glBindTexture(GL_TEXTURE_2D, reflectionTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, reflectionTexture, 0);


	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, attachments);

#ifdef _DEBUG
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw GException("Framebuffer reflection one not complete!");
	GObject::throwError("World():\n");
#endif
	//
	//
	//
	initializePostProcessings();

	initializeAll();

	createObjects();


	GObject::setAvaibleTextureUnit(3);

	pickedRigidBody = static_cast<Moveable*>( point_lights.back().get() );

	player->calculateState();

#ifdef _DEBUG
	gDebug = new GLDebugDrawer();
	gDebug->setDebugMode(1);
	dynamicsWorld->setDebugDrawer(gDebug);
	debug_program = GObject::createProgram("shaders\\world\\renderscene.vert.glsl", "shaders\\world\\renderscene.frag.glsl");
#endif

	//glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);

	glAlphaFunc(GL_GEQUAL, 1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_BLEND);
	//glEnable(GL_FRAMEBUFFER_SRGB);

}

inline void World::initializeAll()
{
	GameObject::initialize(player);
	GObject::initialize(&point_lights, &direc_lights, &spot_lights);

	GBasicLightObject::initialize();
	GBasicTextureLightObject::initialize();
	GTiledTextureObject::intialize();

	GDefaultObject::initialize();
	GDefaultSplatObject::initialize();
	GMapAObject::initialize();
	GMapRObject::initialize();

	GDefaultMultiTextureObject::initialize();

	Sky::initialize();
	Moon::initialize();
	Sun::initialize();

	MapleTree::initialize();
	PineTree::initialize();
	Fern::initialize();
	Log::initialize();
	GreyRock::initialize();
	RedRock::initialize();

	Seon::initialize();

	broadphase = new btDbvtBroadphase();

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = shared_ptr<btDiscreteDynamicsWorld>(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration));
	dynamicsWorld->setGravity(btVector3(0.f, -9.8f, 0.f));

	player->setbtWorld(dynamicsWorld);
	pObject::initialize(dynamicsWorld);

#ifdef _DEBUG
	GObject::throwError("World::initializeAll():\n");
#endif
}

inline void World::createObjects()
{
	objects.emplace_back(new Sky(glm::vec3{ 0,-40,0 }));
	ocean.reset(new GMapRObject("lake", glm::vec3{ 40,0,10 }));	ocean->setReflectionTexture(Texture{ GL_TEXTURE_2D, reflectionTexture });
	objects.push_back(ocean);

	sun = shared_ptr<Sun>(new Sun());
	sun->shadowMappingInitialize(4096, 4096);

	objects.emplace_back(new Moon());


	direc_lights.push_back(static_pointer_cast<DirecLight>(sun));
	objects.push_back(static_pointer_cast<GObject>(sun));
	lights.push_back(static_pointer_cast<Light>(sun));

	//static_objects.emplace_back(new StaticDefaultSplatObject("terrain"));
	objects.emplace_back(new StaticDefaultSplatObject("terrain"
		//, glm::vec3{ 0,20,0 }
	));

	

	objects.emplace_back(new Log(glm::vec3{ 40, heightOf(40, -20) ,-20 }, glm::vec3{ 2.0f }));

	objects.emplace_back(	new GMapAObject("fern", glm::vec3{ 50, heightOf(50, 50) ,50 } )	);
	objects.emplace_back(	new Fern(glm::vec3{ 55, heightOf(55, 50) ,50 }, glm::vec3{ 3.0f }));
	objects.emplace_back(	new Fern(glm::vec3{ 20, heightOf(20, -40) ,-40 }, glm::vec3{ 6.8f }));
	objects.emplace_back(	new Fern(glm::vec3{ 22, heightOf(22, 41) ,41 }, glm::vec3{ 6.0f }));

	objects.emplace_back(	new DefaultStaticObject("rock", glm::vec3{ 5, heightOf(5, 5) ,5 })		);
	//objects.emplace_back(	new DefaultStaticObject("oaktree", glm::vec3{ -4, heightOf(-4, 30) ,30 }));
	//objects.emplace_back(	new GDefaultMultiTextureObject("maple", glm::vec3{ -4, heightOf(-4, 30) ,30 }));
	objects.emplace_back(new MapleTree(glm::vec3{ -4, heightOf(-4, 30) ,30 }, glm::vec3{ 2.0f }));
	objects.emplace_back(new PineTree(glm::vec3{ -23, heightOf(-23, -33) ,-33 }, glm::vec3{ 2.0f }));
	objects.emplace_back(new GreyRock(glm::vec3{ 26, heightOf(26, 29) ,29 }));
	objects.emplace_back(new RedRock(glm::vec3{ 32, heightOf(32, 20) ,20 }));



	shared_ptr<Moon> moon = shared_ptr<Moon>(new Moon());

	direc_lights.push_back(static_pointer_cast<DirecLight>(moon));
	objects.push_back(static_pointer_cast<GObject>(moon));
	lights.push_back(static_pointer_cast<Light>(moon));

	point_lights.emplace_back(new Seon(glm::vec3{ 5.0f,25.0f,5.0f }, glm::vec3{ 1.0f }));
	objects.push_back(dynamic_pointer_cast<GObject>(point_lights.back()));
	lights.push_back(dynamic_pointer_cast<Light>(point_lights.back()));



	flashLight = shared_ptr<FlashLight>(new FlashLight("flashlight", glm::vec3{ 50,20,50 }));
	flashLight->shadowMappingInitialize(1024, 1024);

	spot_lights.push_back(static_pointer_cast<SpotLight>(flashLight));
	objects.push_back(static_pointer_cast<GObject>(flashLight));
	lights.push_back(static_pointer_cast<Light>(flashLight));

#ifdef _DEBUG
	for(int i =0; i < objects.size(); i++)
	{
		objects[i]->d_setObjectIdentity(typeid(*objects[i]).name() + to_string(i) );
		cout << "Object number:" << i << "\t" << objects[i]-> d_getObjectIdentity() << endl;
	}//typeid( this ).hash_code()
	GObject::throwError("World::createObjects():\n");
#endif
}

inline GLfloat World::heightOf(GLfloat x, GLfloat z)
{
	btVector3 start(x, 300, z);
	btVector3 end = start + btVector3(0,-1,0)*350;
	btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
	dynamicsWorld->rayTest(start, end, RayCallback);

	return RayCallback.m_hitPointWorld.getY();
}



inline void World::initializePostProcessings()
{
	program[FINAL] = GObject::createProgram("shaders\\world\\bloom.vert.glsl", "shaders\\world\\bloom.frag.glsl");
	program[BLUR_HOR] = GObject::createProgram("shaders\\world\\blurHorizantal.vert.glsl", "shaders\\world\\blurHorizantal.frag.glsl");
	program[BLUR_VER] = GObject::createProgram("shaders\\world\\blurVertical.vert.glsl", "shaders\\world\\blurVertical.frag.glsl");

	glGenFramebuffers(POST_SIZE, fbo);
	glGenTextures(POST_SIZE, texture);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo[FINAL]);

	for (GLuint i = HDR_0; i <= HDR_1; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i - HDR_0, GL_TEXTURE_2D, texture[i], 0);
	}

	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

#ifdef _DEBUG
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw GException("Framebuffer FINAL one not complete!");
#endif

	for (GLuint i = BLUR_HOR; i <= BLUR_VER; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture[i], 0
		);
#ifdef _DEBUG
		GLuint attachments[1] = { GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, attachments);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw GException("Framebuffer BLUR one not complete!");
#endif
	}




	glGenVertexArrays(POST_SIZE, vao);
	glGenBuffers(POST_SIZE, vbo);


	GLfloat quad[] = {
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
	};

	glBindVertexArray(vao[FINAL]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[FINAL]);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

#ifdef _DEBUG
	GObject::throwError("World::initializePostProcessings():\n");
#endif
}

void World::blurScene()
{

	glBindFramebuffer(GL_FRAMEBUFFER, fbo[BLUR_HOR]);
	// Blur it horozantily
	glUseProgram(program[BLUR_HOR]);

	glActiveTexture(GL_TEXTURE0 + GObject::getAvaibleTextureUnit());
	glBindTexture( GL_TEXTURE_2D, texture[HDR_1] );
	glUniform1i(100, GObject::getAvaibleTextureUnit());
	renderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo[BLUR_VER]);
	// Blur it vertically
	glUseProgram(program[BLUR_VER]);

	glActiveTexture(GL_TEXTURE0 + GObject::getAvaibleTextureUnit());
	glBindTexture(GL_TEXTURE_2D, texture[BLUR_HOR]);
	glUniform1i(100, GObject::getAvaibleTextureUnit());
	renderScene();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#ifdef _DEBUG
	GObject::throwError("World::blurScene():\n");
#endif // _DEBUG
}

inline void World::finalRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(program[FINAL]);
	glActiveTexture(GL_TEXTURE0 + GObject::getAvaibleTextureUnit());
	glBindTexture(GL_TEXTURE_2D, texture[HDR_0]);
	glUniform1i(100, GObject::getAvaibleTextureUnit());

	glActiveTexture(GL_TEXTURE0 + GObject::getAvaibleTextureUnit() +1);
	glBindTexture(GL_TEXTURE_2D, texture[BLUR_VER]);
	glUniform1i(101, GObject::getAvaibleTextureUnit()+1);
	renderScene();
#ifdef _DEBUG
	GObject::throwError("World::finalRender():\n");
#endif // _DEBUG

}

void World::renderScene()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glBindVertexArray(vao[FINAL]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[FINAL]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glUseProgram(0);

}

inline void World::bloomRender()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo[FINAL]);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	allRender();

	blurScene();

	//debugRenderScene(texture[HDR_0]);

	finalRender();
}

inline void World::shadowRender()
{
	//glEnable(GL_ALPHA_TEST);
	glPolygonOffset(0.5, 0.1);

	GObject::setRenderMode(RenderMode::SHADOW_CALC);
	shadowRender((Light*)sun.get(),0);
	GObject::setSunViewMatrix(sun->getLightViewMatrix(), sun->getLightProjMatrix());

	glPolygonOffset(0.3, 0.0);

	flashLight->setNoRender( true );
	shadowRender((Light*)flashLight.get(), 1);
	GObject::setSpotlightViewMatrix(flashLight->getLightViewMatrix(), flashLight->getLightProjMatrix());
	flashLight->setNoRender(false);


	GObject::setRenderMode(renderMode);
	//glDisable(GL_ALPHA_TEST);

}

inline void World::reflectionRender()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	GObject::setRenderMode(RenderMode::REFLECTION_CALC);
	GObject::setReflectionMatrix(glm::scale(glm::vec3{ 1,-1,1 }), player->getSeaReflectedView(), player->getProjMatrix(), glm::vec4{ 0,0,0,0 });
	allRender();
	GObject::setRenderMode(renderMode);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

inline void World::shadowRender(Light * light, unsigned int unit)
{
	light->startMapping();
	GObject::setLightViewMatrix(light->getLightViewMatrix(), light->getLightProjMatrix());
	allRender();
	light->finishMapping(unit);
}


inline void World::allRender()
{
	try {

#ifdef _DEBUG
		if (debugDrawOn)
		{

			gDebug->pushMatrix(player->getViewProjMatrix());
			dynamicsWorld->debugDrawWorld();
			gDebug->popMatrix();
			GObject::throwError("debugDrawWorld():\n");

		}
#endif

		for (auto o : objects)
		{
 			o->render();
		}


	}
	catch (GException e) {
		cerr << "World::render():\n" << e.what() << endl;
	}
}

inline void World::stepSimulation(const double & deltaTime)
{
	GameObject::setSimulationStep(deltaTime);
	player->stepSimulation(deltaTime);
	dynamicsWorld->stepSimulation(deltaTime);

	player->calculateViewMatrix();
	calculateButtonsStates(deltaTime);
	calculateLights(deltaTime);

	GObject::setLights();

}



void World::render(const double & deltaTime)
{
#ifdef _DEBUG
	try {
#endif
		if(GameObject::isDay())	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		else glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		stepSimulation(deltaTime);

#ifdef _DEBUG
	}
	catch (GException e)
	{
		cout << e.what() << endl;
	}
	static double timepast = 0;
	timepast += deltaTime;
	if (timepast >= 0.7) {		cout << player->getPosition() << endl; timepast = 0;	}

if (debugRenderOn)
{
	flashLight->setNoRender(true);

	calculateDebugView();
	for (auto o : objects)
		o->debugRender();

	flashLight->setNoRender(false);



}
else 
{
#endif

	reflectionRender();
	shadowRender();
	bloomRender();

#ifdef _DEBUG
}
try {
	GObject::throwError("World::render()[exit]:\n");
}
catch (GException e)
{
	cout << e.what() << endl;
}
#endif
}


void World::processMouseOffsets(double xOffset, double yOffset)
{
	switch (tool)
	{
	case Tools::Pick:
		if (pickedRigidBody != NULL)
		{
			if (buttonStateRight == RELEASE)
			{
				pickedRigidBody->translateOn(xOffset*0.05, 0, yOffset*0.05, player->getDirections());

			}
			else
			{
				double x = xOffset*0.1;	double y = yOffset*0.1;
				pickedRigidBody->rotateAround(y, x, 0.0, player->getDirections());

			}
		}
		break;
	case Tools::Toggle:
		break;
	case Tools::Seon:
		if (pickedRigidBody != NULL && buttonStateLeft == TOGGLE)
		{
			pickedRigidBody->translateOn(xOffset*0.05, 0, yOffset*0.05, player->getDirections());

		}
		break;
	case Tools::LightPick:
		break;
	}


	player->processMouseMovement(xOffset, yOffset);

}

void World::processScrollOffsets(double xOffset, double yOffset)
{
	if ( pickedRigidBody != NULL  )
	{
		double y = yOffset;
		pickedRigidBody->translateOn(0, y, 0, player->getDirections() );

	}
	if (pickedLight != NULL)
	{
		if (yOffset < 0)
			pickedLight->decreaseLightStrength();
		else if (yOffset > 0)
			pickedLight->increaseLightStrength();
	}
}

inline void World::resetTools(Tools last, Tools current)
{
	if (last != current)
	{
		switch (last)
		{
		case Tools::Pick: 
			pickedRigidBody = nullptr;
			break;
		case Tools::Seon:
			
			break;
		case Tools::Toggle:
			toggled_objs.clear();
			break;
		case Tools::LightPick:
			pickedLight = nullptr;
			break;
		}
	}
}

inline void World::toggle()
{
	if (buttonStateLeft == PRESS)
	{
		btVector3 start, end;
		player->getForwardXYZRay(40, start, end);
		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
		dynamicsWorld->rayTest(start, end, RayCallback);

		pObject * obj = pObject::getpObject(RayCallback.m_collisionObject);
		Moveable * mov = dynamic_cast< Moveable* >(obj);
		if (mov != nullptr)
		{

			bool add = true;
			for (unsigned int i = 0; i < toggled_objs.size(); i++)
			{
				if ( toggled_objs[i] == mov )
				{
					add = false;
					break;
				}
			}
			if (add)
			{
			#ifdef _DEBUG
				if (obj != nullptr) cout << obj << typeid(*obj).name() << endl; else cout << "Toggle raycast hit nothing" << endl;
			#endif
				toggled_objs.emplace_back(mov);
			}

		}
	}
	if (buttonStateRight == PRESS)
	{
		btVector3 start, end;
		player->getForwardXYZRay(40, start, end);
		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
		dynamicsWorld->rayTest(start, end, RayCallback);

		const pObject * obj = pObject::getpObject(RayCallback.m_collisionObject);
		const Moveable * mov = dynamic_cast<const Moveable*>(obj);
#if(DEBUG_LVL>1)
		if (obj != nullptr) cout << obj << typeid(*obj).name() << endl; else cout << "No Hit" << endl;
#endif
		if (mov != nullptr)
		{

			for (unsigned int i = 0; i < toggled_objs.size(); i++)
			{
				if ( toggled_objs[i] == mov)
				{
					toggled_objs.erase( toggled_objs.begin() + i );
					break;
				}
			}
		}
	}
}

inline void World::seon()
{
	if (buttonStateRight != RELEASE)
		return;

	switch (buttonStateLeft)
	{
	case PRESS:
	if(point_lights.size() < 10)
	{
		glm::vec3 position = player->getPosition() + player->getForwardDirection() * 10;

		//cout << "Player position:" << position << endl;

		point_lights.emplace_back(new Seon( position, glm::vec3{ 1.0f }));
		objects.push_back(dynamic_pointer_cast<GObject>(point_lights.back()));
		lights.push_back(dynamic_pointer_cast<Light>(point_lights.back()));

		//cout << "new seon" << endl;
		pickedRigidBody = dynamic_cast<Moveable*>( point_lights.back().get() );
	}
		break;
	case RELEASE:
		pickedRigidBody = nullptr;
		break;
	}
	switch (buttonStateRight)
	{
	case PRESS:
	{
		btVector3 start, end;
		player->getForwardXYZRay(40, start, end);
		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
		dynamicsWorld->rayTest(start, end, RayCallback);

		pObject * obj = pObject::getpObject(RayCallback.m_collisionObject);


		//cout << "new seon" << endl;
		Seon * seon = dynamic_cast<Seon*>(obj);
	}

	}

}

inline void World::pick()
{
	switch (buttonStateLeft)
	{
	case PRESS:
	{
		btVector3 start, end;
		player->getForwardXYZRay(40, start, end);
		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
		dynamicsWorld->rayTest(start, end, RayCallback);

		pObject * obj = pObject::getpObject(RayCallback.m_collisionObject);
		Moveable * mov = dynamic_cast< Moveable* >(obj);
#ifdef _DEBUG
		if (obj != nullptr)cout << obj <<"||"<< typeid(*obj).name() << endl; else cout << "Raycast hit nothing" << endl;
		if (mov != nullptr)cout << "Object is moveable, it`s picked" << "||" << typeid(*obj).name() << endl; else cout << "Object is not moveable." << endl;
#endif
		if (mov != nullptr)
		{

			pickedRigidBody =  mov ;
		}
	}
		break;
	case RELEASE:
		pickedRigidBody = nullptr;
		break;
	}
}

inline void World::lightPick()
{
	if (buttonStateLeft == PRESS)
	{
		btVector3 start, end;
		player->getForwardXYZRay(40, start, end);
		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
		dynamicsWorld->rayTest(start, end, RayCallback);

		pObject * obj = pObject::getpObject(RayCallback.m_collisionObject);

#if (DEBUG_LVL>=1)
		if (obj != nullptr)cout << obj << typeid(*obj).name() << " || HashCode:" << typeid(*obj).hash_code() << endl; else cout << "Light raycast Raycast hit nothing" << endl;
#endif
		Light * light = dynamic_cast< Light* >(obj);

		if (light != nullptr)
		{
			pickedLight = light;
		}
	}
	if (buttonStateRight == PRESS)
	{
		pickedLight = nullptr;

	}
}

inline void World::calculateButtonsStates(double deltaTime)
{
	static double new_light_time = 5;
	new_light_time += deltaTime;
	switch (tool)
	{
	case Tools::Pick:
		pick();
		break;
	case Tools::Seon:
		seon();
		break;
	case Tools::Toggle:
		toggle();
		break;
	case Tools::LightPick:
		lightPick();
		break;
	}
}

inline void World::calculateLights(double deltaTime)
{
	for (auto light : lights)
	{
		light->calculateLightening();
	}
}



void World::processKeys(const bool keys[], const double & deltaTime)
{
	static float toggle_time_renderMode = 0.0f;	static char m = 2 ;
	static float toggle_time_tools = 0.0f;
	//static float toggle_time_jump = 0.0f;

	static bool last_space= false;
	toggle_time_renderMode += deltaTime;	toggle_time_tools += deltaTime;


	glm::vec3 deltaPos;
	CameraMovement directions = CameraMovement::NO_MOVE;

	if (keys[GLFW_KEY_F1])
		player->setMode(CameraMode::DEFAULT);
	if (keys[GLFW_KEY_F2])
		player->setMode(CameraMode::GHOST);
	if (keys[GLFW_KEY_F3])
		player->setMode(CameraMode::FLY);

#ifdef _DEBUG
	if (keys[GLFW_KEY_F10])
	{
		if (toggle_time_renderMode > 1)
		{
			debugDrawOn = !debugDrawOn;
			toggle_time_renderMode = 0;
		}
	}
	if (keys[GLFW_KEY_F11])
	{
		if (toggle_time_renderMode > 1)
		{
			debugRenderOn = !debugRenderOn;
			toggle_time_renderMode = 0;
		}
	}
	if (keys[GLFW_KEY_F12])
	{
		if (toggle_time_renderMode > 1)
		{
			loopDebugView();
			toggle_time_renderMode = 0;
		}
	}

#endif

	if (keys[GLFW_KEY_F9])
	{
		if (toggle_time_renderMode > 1)
		{
			renderMode = (renderMode + 1) % RenderMode::SHADOW_CALC;
			cout << static_cast<int>( renderMode )<< endl;

			toggle_time_renderMode = 0;
		}
	}
	if (keys[GLFW_KEY_F8])
	{
		if (toggle_time_renderMode > 1)
		{
			renderMode = RenderMode::DEFAULT;
			toggle_time_renderMode = 0;
		}
	}

	if (keys[GLFW_KEY_F7])
	{
		if (toggle_time_renderMode > 1)
		{
			static bool day = true;
			if (day) { GameObject::setDegreeOfDay(0); day = false; }
			else { GameObject::setDegreeOfDay(180); day = true; }
			toggle_time_renderMode = 0;
		}
	}

	if (keys[GLFW_KEY_X])
	{
		if (toggle_time_renderMode > 1)
		{
			flashLight->toggleOnHand();
			toggle_time_renderMode = 0;
		}
	}
	if (keys[GLFW_KEY_F])
	{
		if (toggle_time_renderMode > 1)
		{
			flashLight->toggleActive();
			toggle_time_renderMode = 0;
		}
	}
	if (keys[GLFW_KEY_T])
	{
		if (toggle_time_renderMode > 1 && pickedLight !=nullptr)
		{
			pickedLight->toggleActive();
			toggle_time_renderMode = 0;
		}
	}

	Speed sp = Speed::NORMAL;
	if (keys[GLFW_KEY_LEFT_SHIFT]) {
		sp = Speed::FAST;
	}
	if (keys[GLFW_KEY_LEFT_ALT]) {
		sp = Speed::SLOW;
	}


	
	if (keys[GLFW_KEY_W])
		directions = directions | CameraMovement::FORWARD;
	if (keys[GLFW_KEY_S])
		directions = directions | CameraMovement::BACKWARD;
	if (keys[GLFW_KEY_A])
		directions = directions | CameraMovement::LEFT;
	if (keys[GLFW_KEY_D])
		directions = directions | CameraMovement::RIGHT;

	if (player->getCameraMode() == CameraMode::DEFAULT)
	{
		if (keys[GLFW_KEY_SPACE] && player->canJump() && !last_space)
		{
			if (keys[GLFW_KEY_LEFT_CONTROL])
				sp = Speed::FAST;
			player->startJump( directions, sp );
		}
		else if(player->onGround())
		{
			deltaPos = player->processMovement(directions, deltaTime, sp);
			if (toggled_objs.size() > 0 )
				for(auto obj : toggled_objs)
					obj->translate(deltaPos);
		}
			
	}
	else
	{
		if (keys[GLFW_KEY_UP])
			directions = directions | CameraMovement::PITCH_POS;
		if (keys[GLFW_KEY_DOWN])
			directions = directions | CameraMovement::PITCH_NEG;

		if (keys[GLFW_KEY_LEFT])
			directions = directions | CameraMovement::YAW_NEG;
		if (keys[GLFW_KEY_RIGHT])
			directions = directions | CameraMovement::YAW_POS;

		if (keys[GLFW_KEY_E])
			directions = directions | CameraMovement::ROLL_POS;
		if (keys[GLFW_KEY_Q])
			directions = directions | CameraMovement::ROLL_NEG;

		if (keys[GLFW_KEY_LEFT_CONTROL])
			directions = directions | CameraMovement::DOWN;

		if (keys[GLFW_KEY_SPACE])
			directions = directions | CameraMovement::UP;

		player->processMovement(directions, deltaTime, sp);

	}

	//bitset<sizeof(12)>x(directions);
	//cout <<  x  << endl;
	static Tools last_tool=Tools::Pick;
	static unsigned int option = 0;
	if (toggle_time_tools>0.5f)
	{
		if (keys[GLFW_KEY_1]) {
			tool = Tools::Pick;
			if (last_tool != Tools::Pick)
				option = 0;
			toggle_time_tools = 0.0f;
			last_tool = Tools::Pick;

		}
		else if (keys[GLFW_KEY_2]) {
			tool = Tools::Toggle;
			if (last_tool != Tools::Toggle)
				option = 0;
			this->option = Options::Toggle_Rotate + option;
			option = (option+1) % 2;
			toggle_time_tools = 0.0f;
			last_tool = Tools::Toggle;

		}
		else if (keys[GLFW_KEY_3]) {
			tool = Tools::Seon;
			if (last_tool != Tools::Seon)
				option = 0;
			this->option  = Options::Seon_White + option;
			option = (option+1) % 4;
			toggle_time_tools = 0.0f;
			last_tool = Tools::Seon;
		}
		else if (keys[GLFW_KEY_4]) {
			tool = Tools::LightPick;
			if (last_tool != Tools::LightPick)
				option = 0;
			toggle_time_tools = 0.0f;
			last_tool = Tools::LightPick;

		}


	}

	if ( toggled_objs.size() > 0 ) {
		//cout << typeid(*picked_obj).name() << endl;

		if ( this->option == Options::Toggle_Rotate )
		{
			double x = 0, y = 0, z = 0;
			if (keys[324])
				y += 45 * deltaTime;
			if (keys[326])
				y -= 45 * deltaTime;	
			if (keys[322])
				x -= 45*deltaTime;
			if (keys[328])
				x += 45 * deltaTime;
			if (keys[327])
				z -= 45 * deltaTime;
			if (keys[329])
				z += 45 * deltaTime;

			if (x != 0 || y != 0 || z != 0)
				for (auto obj : toggled_objs)
					obj->rotate(x, y, z);

			//cout << "Keys of rotate" << endl;
		}
		else if ( this->option == Options::Toggle_Translate )
		{
			double x = 0, y = 0, z = 0;
				if (keys[324])
					x -= 1 * deltaTime;
				if (keys[326])
					x += 1 * deltaTime;
				if (keys[321])
					y -= 1 * deltaTime;
				if (keys[327])
					y += 1 * deltaTime;
				if (keys[322])
					z -= 1 * deltaTime;
				if (keys[328])
					z += 1 * deltaTime;

				if (x != 0 || y != 0 || z != 0)
					for( auto obj : toggled_objs )
						obj->translate(x, y, z);

				//cout << "Keys of translate" << endl;

		}
	}

	last_space = keys[GLFW_KEY_SPACE];
}

void World::processMouseButtons(int state_left, int state_middle, int state_right)
{
	switch (state_left) {
	case GLFW_PRESS:
		switch (buttonStateLeft) {
		case PRESS:
			buttonStateLeft = TOGGLE;
			break;
		case RELEASE:
			buttonStateLeft = PRESS;
			break;
		}
		break;
	case GLFW_RELEASE:
		switch (buttonStateLeft) {
		case PRESS:
			buttonStateLeft = RELEASED;
			break;
		case TOGGLE:
			buttonStateLeft = RELEASED;
			break;
		case RELEASED:
			buttonStateLeft = RELEASE;
			break;
		}
		break;
	}

	switch (state_middle) {
	case GLFW_PRESS:
		switch (buttonStateMiddle) {
		case PRESS:
			buttonStateMiddle = TOGGLE;
			break;
		case RELEASE:
			buttonStateMiddle = PRESS;
			break;
		}
		break;
	case GLFW_RELEASE:
		switch (buttonStateMiddle) {
		case PRESS:
			buttonStateMiddle = RELEASED;
			break;
		case TOGGLE:
			buttonStateMiddle = RELEASED;
			break;
		case RELEASED:
			buttonStateMiddle = RELEASE;
			break;
		}
		break;
	}
	switch (state_right) {
	case GLFW_PRESS:
		switch (buttonStateRight) {
		case PRESS:
			buttonStateRight = TOGGLE;
			break;
		case RELEASE:
			buttonStateRight = PRESS;
			break;
		}
		break;
	case GLFW_RELEASE:
		switch (buttonStateRight) {
		case PRESS:
			buttonStateRight = RELEASED;
			break;
		case TOGGLE:
			buttonStateRight = RELEASED;
			break;
		case RELEASED:
			buttonStateRight = RELEASE;
			break;
		}
		break;
	}

#if (DEBUG_LVL>=2)
	cerr << "States:\n" << state_left << endl << state_middle << endl << state_right << endl;
	cerr << "States:\n" << buttonStateLeft << endl << buttonStateMiddle << endl << buttonStateRight << endl;
#endif

}


World::~World()
{
	delete player;
}
#ifdef _DEBUG
void World::calculateDebugView()
{
	switch (debugView)
	{
	case 0:
		GObject::setDebugViewProjMatrix(player->getSeaReflectedView(), player->getProjMatrix());
		break;
	case 1:
		GObject::setDebugViewProjMatrix(sun->getLightViewMatrix(), sun->getLightProjMatrix());
		break;
	case 2:
		GObject::setDebugViewProjMatrix(flashLight->getLightViewMatrix(), flashLight->getLightProjMatrix());
		break;
	}
}
void World::loopDebugView()
{
	switch (debugView)
	{
	case 0:
		debugView = 1;
		break;
	case 1:
		debugView = 2;
		break;
	case 2:
		debugView = 0;
		break;
	}
}
void World::debugRenderScene(GLuint texture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(debug_program);
	glActiveTexture(GL_TEXTURE0 + GObject::getAvaibleTextureUnit());
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(100, GObject::getAvaibleTextureUnit());
	renderScene();
}
#endif
