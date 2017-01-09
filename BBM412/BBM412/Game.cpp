#include "Game.h"

Game * Game::curr_game;
GLFWwindow * Game::current_window;
World * Game::current_world;
Console * Game::current_console;

Game::Game()
{
	if (!glfwInit())
		std::cout << "Failed to initialize GLFW" << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

	window = glfwCreateWindow(1600, 800, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		cerr << "WINDOW Could not initiliazed\n";
	}

	glfwSetKeyCallback(window, Game::key_callback);
	glfwSetCursorPosCallback(window, Game::mouse_callback);
	glfwSetMouseButtonCallback(window, Game::mouse_button_callback);
	glfwSetScrollCallback(window, Game::scroll_callback);

	//Initial position of mouse offsets.
	glfwGetCursorPos(window, &lastX, &lastY);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);
	//For 60Hz refresh rate.
	glfwSwapInterval(1);

	if (Game::curr_game != nullptr) delete Game::curr_game;

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
	}
#ifdef _DEBUG
	int err;
	if ((err = glGetError()) != GL_NO_ERROR) {
		cout << "GlewInit has an error.. how interesting!" << endl << GObject::returnErrorString(err);
		while ((err = glGetError()) != GL_NO_ERROR)
			cout << endl << GObject::returnErrorString(err);
	}
	cout << glGetString(GL_VENDOR) << endl << glGetString(GL_RENDERER) << endl;
	cout << glGetString(GL_VERSION) << endl << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	cout << "Max Texture Units:" << GL_MAX_TEXTURE_UNITS << endl;

#endif
	world = new World();


	curr_game = this;
	current_world = this->world;
	current_console = this->console;
	current_window = this->window;
}


int Game::run()
{
	// GLFW_KEY_GRAVE_ACCENT for console

	if (curr_game == nullptr)
		return -1;

	while (!glfwWindowShouldClose(current_window))
	{
		double currentFrame = glfwGetTime();
		curr_game->deltaTime = currentFrame - curr_game->lastFrame;
		curr_game->lastFrame = currentFrame;

		glfwPollEvents();

		current_world->processMouseButtons(curr_game->stateLeft, curr_game->stateMiddle, curr_game->stateRight);
		current_world->processKeys(curr_game->keys, curr_game->deltaTime);

		current_world->render(curr_game->deltaTime);

		glFlush();

		glfwSwapBuffers(current_window);

	}
	delete curr_game;
	return 0;
}

void Game::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	current_world->processScrollOffsets(xoffset, yoffset);
}

void Game::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) {
			curr_game->keys[key] = true;
#if (DEBUG_LVL>=2)
			cout << key << ":PRESSED" << endl;
#endif
		}
		else if (action == GLFW_RELEASE) {

			curr_game->keys[key] = false;
		}
	}
}

void Game::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	double xOffset = xpos - curr_game->lastX;
	double yOffset = curr_game->lastY - ypos;

	curr_game->lastX = xpos;
	curr_game->lastY = ypos;


#if (DEBUG_LVL>=2)
	cout << xpos << "," << ypos << endl;
	cout << xOffset << "," << yOffset << endl;
#endif

	current_world->processMouseOffsets(xOffset, yOffset);
}

void Game::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	switch (button) {
	case GLFW_MOUSE_BUTTON_RIGHT:
		switch (action) {
		case GLFW_PRESS:
			curr_game->stateRight = GLFW_PRESS;

			break;
		case GLFW_RELEASE:
			curr_game->stateRight = GLFW_RELEASE;
			break;
		}
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		switch (action) {
		case GLFW_PRESS:
			curr_game->stateMiddle = GLFW_PRESS;
			break;
		case GLFW_RELEASE:
			curr_game->stateMiddle = GLFW_RELEASE;
			break;
		}
		break;
	case GLFW_MOUSE_BUTTON_LEFT:
		switch (action) {
		case GLFW_PRESS:
			curr_game->stateLeft = GLFW_PRESS;
			break;
		case GLFW_RELEASE:
			curr_game->stateLeft = GLFW_RELEASE;
			break;
		}
		break;
	}
}


Game::~Game()
{
	if (this == curr_game)
	{
		curr_game = nullptr;
		current_world = nullptr;
		current_console = nullptr;
		current_window = nullptr;
	}

	delete world;
	delete console;
	glfwTerminate();
}
