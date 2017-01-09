#include "World.h"
#include <iostream>
#include <bitset>



World::World(unsigned int screen_width, unsigned int screen_height) :
	SCREEN_WIDTH(screen_width), SCREEN_HEIGHT(screen_height),
	player(new pPerson{ dynamicsWorld, glm::vec3{ -10.0,100,-30.0 } }),
	sun(new Sun())
{
	initializePostProcessings();

	initializeAll();
	createObjects();

	picked_obj = static_pointer_cast<Moveable>(spot_lights[0]);

	player->calculateState();

#ifdef _DEBUG
	gDebug = new GLDebugDrawer();
	gDebug->setDebugMode(1);
	dynamicsWorld->setDebugDrawer(gDebug);
#endif

	//glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_FRAMEBUFFER_SRGB);

#ifdef _DEBUG
	GObject::throwError("World():\n");
#endif
}

inline void World::initializeAll()
{
	GameObject::initialize(player);
	GObject::initialize(&point_lights, &direc_lights, &spot_lights);
	GBasicLightObject::initialize();

	GDefaultObject::initialize();

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
	static_objects.emplace_back(new DefaultStaticObject("terrain"));
	objects.push_back(static_pointer_cast<GObject>(static_objects.back()));

	objects.emplace_back(new GBasicTextureObject("sky"));

	static_objects.emplace_back(new DefaultStaticObject("rock", glm::vec3{ 5,5,5 }));
	objects.push_back(static_pointer_cast<GObject>(static_objects.back()));

	dynamic_objects.emplace_back(new DefaultDynamicObject("rock"));
	objects.push_back(static_pointer_cast<GObject>(dynamic_objects.back()));

	direc_lights.push_back(sun);
	objects.push_back(sun);
	lights.push_back(sun);

#ifdef _DEBUG
	GObject::throwError("World::createObjects()2:\n");
#endif

	point_lights.emplace_back(new Seon(glm::vec3{ 5.0f,5.0f,5.0f }, glm::vec3{ 1.0f }));
	objects.push_back(dynamic_pointer_cast<GObject>(point_lights.back()));
	lights.push_back(dynamic_pointer_cast<Light>(point_lights.back()));


#ifdef _DEBUG
	GObject::throwError("World::createObjects()1:\n");
#endif

	spot_lights.emplace_back(
		new FlashLight("flashlight", glm::vec3{ 5,0,0 })
	);
	objects.push_back(dynamic_pointer_cast<GObject>(spot_lights.back()));
	flashLight = dynamic_pointer_cast<FlashLight>(spot_lights.back());
	lights.push_back(dynamic_pointer_cast<Light>(spot_lights.back()));

#ifdef _DEBUG
	GObject::throwError("World::createObjects():\n");
#endif
}

inline GLfloat World::heightOf(GLfloat x, GLfloat z)
{
	btVector3 start(x, 200.0, z);
	btVector3 end = start + btVector3(0,-1,0)*200;
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture[i], 0);
	}
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
#ifdef _DEBUG
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
#endif

	for (GLuint i = BLUR_VER; i <= BLUR_HOR; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);
		glBindTexture(GL_TEXTURE_2D, fbo[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo[i], 0
		);
#ifdef _DEBUG
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
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
	// bind texture of other framebuffer (or scene if first iteration)
	glUseProgram(program[BLUR_HOR]);

	glActiveTexture(GL_TEXTURE0 + GObject::getAvaibleTextureUnit());
	glBindTexture( GL_TEXTURE_2D, texture[HDR_1] );
	glUniform1i(100, GL_TEXTURE0 + GObject::getAvaibleTextureUnit());
	renderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo[BLUR_VER]);
	// bind texture of other framebuffer (or scene if first iteration)
	glUseProgram(program[BLUR_HOR]);

	glActiveTexture(GL_TEXTURE0 + GObject::getAvaibleTextureUnit());
	glBindTexture(GL_TEXTURE_2D, texture[BLUR_HOR]);
	glUniform1i(100, GL_TEXTURE0 + GObject::getAvaibleTextureUnit());
	renderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

inline void World::finalRender()
{
	glUseProgram(program[FINAL]);
	glActiveTexture(GL_TEXTURE0 + GObject::getAvaibleTextureUnit());
	glBindTexture(GL_TEXTURE_2D, texture[HDR_0]);
	glUniform1i(100, GL_TEXTURE0 + GObject::getAvaibleTextureUnit());

	glActiveTexture(GL_TEXTURE0 + GObject::getAvaibleTextureUnit());
	glBindTexture(GL_TEXTURE_2D, texture[BLUR_VER]);
	glUniform1i(101, GL_TEXTURE0 + GObject::getAvaibleTextureUnit());
	renderScene();
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

inline void World::shadowRender()
{
	GObject::setRenderMode(RenderMode::SHADOW_CALC);
	shadowRender((Light*)&sun);
	GObject::setRenderMode(RenderMode::DEFAULT);

}

inline void World::shadowRender( Light * light )
{
	light->startMapping();
	GObject::setLightViewMatrix( light->getLightViewMatrix(), light->getLightProjMatrix() );
	allRender();
	light->finishMapping(0);
}

inline void World::allRender()
{
	try {

		for (auto o : objects)
		{
			o->render();
		}
#ifdef _DEBUG
		if (debugDrawOn)
		{

			gDebug->pushMatrix(player->getViewProjMatrix());
			dynamicsWorld->debugDrawWorld();
			gDebug->popMatrix();
			GObject::throwError("debugDrawWorld():\n");

		}
#endif

	}
	catch (GException e) {
		cerr << "World::render():\n" << e.what() << endl;
	}
}

inline void World::stepSimulation(const double & deltaTime)
{
	GameObject::stepSimulation(deltaTime);
	player->stepSimulation(deltaTime);
	//dynamicsWorld->stepSimulation(deltaTime);

	player->calculateViewMatrix();
	calculateButtonsStates(deltaTime);
	calculateLights(deltaTime);

	GObject::setLights();

	shadowRender();
}



void World::render(const double & deltaTime)
{

	GObject::throwError("World::render()[enter]:\n");

#ifdef _DEBUG
	//cout << "World:render():\n" << endl;
	//gDebug->drawTriangle(btVector3(-100, -5, 5), btVector3(0, 100, 5), btVector3(100, -100, 5), btVector3(1, 0, 0), btScalar(1));

#endif
	glClearColor(0.0f, 0.0f, 0.8f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	stepSimulation(deltaTime);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo[FINAL]);
	allRender();

	blurScene();

	finalRender();


}


void World::processMouseOffsets(double xOffset, double yOffset)
{
	if (picked_obj != NULL)
	{
		if (buttonStateRight == RELEASE)
		{
			picked_obj->translateOn(xOffset*0.05, 0, yOffset*0.05, player->getDirections() );
			
		}
		else
		{		
			double x = xOffset*0.1;	double y = yOffset*0.1;
			picked_obj->rotateAround( y, x, 0.0, player->getDirections()	);

		}
	}

	player->processMouseMovement(xOffset, yOffset);

}

void World::processScrollOffsets(double xOffset, double yOffset)
{
	if ( picked_obj != NULL  )
	{
		double y = yOffset;
		picked_obj->translateOn(0, y, 0, player->getDirections() );

	}
}

inline void World::resetTools(Tools last, Tools current)
{
	if (last != current)
	{
		switch (last)
		{
		case Tools::Pick: 
			picked_obj.reset();
			break;
		case Tools::Seon:

			break;
		case Tools::Toggle:
			toggled_objs.clear();
			break;
		}
	}
}

inline void World::toggle()
{
	if (buttonStateLeft == PRESS)
	{
		btVector3 start, end;
		player->getForwardRay(40, start, end);
		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
		dynamicsWorld->rayTest(start, end, RayCallback);

		pObject * obj = pObject::getpObject(RayCallback.m_collisionObject);
		cout << obj << typeid(*obj).name() << endl;
		Moveable * mov = dynamic_cast< Moveable* >(obj);
		bool add = true;
		for (unsigned int i = 0; i < toggled_objs.size(); i++)
		{
			if (toggled_objs[i].get() == mov)
			{
				add = false;
				break;
			}
		}
		if (add)
		{
			toggled_objs.emplace_back(mov);
		}

	}
	if (buttonStateRight == PRESS)
	{
		btVector3 start, end;
		player->getForwardRay(40, start, end);
		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
		dynamicsWorld->rayTest(start, end, RayCallback);

		const pObject * obj = pObject::getpObject(RayCallback.m_collisionObject);
		cout << obj << typeid(*obj).name() << endl;
		const Moveable * mov = dynamic_cast<const Moveable*>(obj);

		for (unsigned int i = 0; i < toggled_objs.size(); i++)
		{
			if (toggled_objs[i].get() == mov)
			{
				toggled_objs.erase(toggled_objs.begin(), toggled_objs.begin() + i);
				break;
			}
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
		player->getForwardRay(40, start, end);
		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
		dynamicsWorld->rayTest(start, end, RayCallback);

		pObject * obj = pObject::getpObject(RayCallback.m_collisionObject);
		cout << obj << typeid(*obj).name() << endl;
		Moveable * mov = dynamic_cast< Moveable* >(obj);
		if (mov != nullptr)
		{
			picked_obj.reset( mov );
		}
	}
		break;
	case TOGGLE:
		break;
	case RELEASE:
		picked_obj.reset();
		break;
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
		break;
	case Tools::Toggle:
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
	static RenderMode last_rm = RenderMode::DEFAULT;
	static float toggle_time_renderMode = 0.0f;	static char m = 2 ;
	static float toggle_time_tools = 0.0f;
	//static float toggle_time_jump = 0.0f;

	static bool last_space= FALSE;
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
	if (keys[GLFW_KEY_F11])
	{
		if (toggle_time_renderMode > 1)
		{
			debugDrawOn = !debugDrawOn;
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

	if (keys[GLFW_KEY_F10])
	{
		if (toggle_time_renderMode > 1)
		{
			last_rm = last_rm + 1;
			//cout << static_cast<int>( last_rm )<< endl;

			GObject::setRenderMode(last_rm);
			toggle_time_renderMode = 0;
		}
	}
	if (keys[GLFW_KEY_F9])
	{
		if (toggle_time_renderMode > 1)
		{
			GObject::setRenderMode(RenderMode::DEFAULT);
			toggle_time_renderMode = 0;
		}
	}


	if (keys[GLFW_KEY_X])
	{
		if (toggle_time_renderMode > 1)
		{
			flashLight->toggleOnHand();
			dynamic_pointer_cast<DefaultDynamicObject>(spot_lights[0])->toggleToggleMode();
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


	if (toggle_time_tools>0.5f)
	{
		if (keys[GLFW_KEY_1]) {
			tool = Tools::Pick;
			toggle_time_tools = 0.0f;
		}
		else if (keys[GLFW_KEY_2]) {
			tool = Tools::Toggle;
			toggle_time_tools = 0.0f;
		}
		else if (keys[GLFW_KEY_3]) {

			toggle_time_tools = 0.0f;
		}
		else if (keys[GLFW_KEY_4]) {
			tool = Tools::Seon;
			option = Options::Seon_White;
			/*
			glm::vec3 pos = player->getPosition()+ player->getForwardDirection()*10;
				
			point_lights.emplace_back(new GSeon(pos, glm::vec3{ 1,0,0.0f }));
			objects.push_back(dynamic_pointer_cast<GObject>(point_lights.back()));

			picked_obj = static_pointer_cast<Moveable>(point_lights.back());
			cout << typeid(*picked_obj).name() << endl;
			*/
			toggle_time_tools = 0.0f;
		}


	}

	if ( toggled_objs.size() > 0 ) {
		//cout << typeid(*picked_obj).name() << endl;

		if ( option == Options::Toggle_Rotate )
		{
			double x = 0, y = 0, z = 0;
			if (keys[324])
				x -= 45*deltaTime;
			if (keys[326])
				x += 45 * deltaTime;
			if (keys[322])
				y -= 45 * deltaTime;	
			if (keys[328])
				y += 45 * deltaTime;
			if (keys[327])
				z -= 45 * deltaTime;
			if (keys[329])
				z += 45 * deltaTime;

			if (x != 0 || y != 0 || z != 0)
				for (auto obj : toggled_objs)
					obj->rotate(x, y, z);

			//cout << "Keys of rotate" << endl;
		}
		else if ( option == Options::Toggle_Translate )
		{
			double x = 0, y = 0, z = 0;
				if (keys[324])
					x += 1 * deltaTime;
				if (keys[326])
					x -= 1 * deltaTime;
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
void World::loopDebugView()
{
	static int index = 0;
	switch (index)
	{
	case 0:
		GObject::setDebugViewProjMatrix(sun->getLightViewMatrix(), sun->getLightProjMatrix() );
		index = 1;
		break;
	case 1:

		index = 0;
		break;
	}
}
#endif
