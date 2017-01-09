#pragma once

#include <glew.h>
#include <glfw\glfw3.h>
#include "World.h"
#include "Console.h"

class Game
{
	GLFWwindow* window;
	bool keys[1024];

	double deltaTime = {0.0f};
	double lastFrame = {0.0f};

	double lastX{ 0.0 }, lastY{ 0.0 };

	int stateRight{ GLFW_RELEASE }, stateMiddle{ GLFW_RELEASE }, stateLeft{ GLFW_RELEASE };

	World * world;
	Console * console;

	static Game * curr_game;
	static GLFWwindow * current_window;
	static World * current_world;
	static Console * current_console;

public:
	Game();

	static int run();

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	~Game();
};

