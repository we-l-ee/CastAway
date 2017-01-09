#include <windows.h>

#include "glew.h"
#include "stddef.h"

#include <glfw\glfw3.h>


#include <iostream>

#include "World.h"
#include "GExceptions.h"

#include "glm\ext.hpp"

using namespace std;

World *world;
bool keys[1024];

double deltaTime = 0.0f;
double lastFrame = 0.0f;

double lastX , lastY ;


bool rightPressed{ false }, middlePressed{ false }, leftPressed{ false };

int stateRight{ GLFW_RELEASE }, stateMiddle{ GLFW_RELEASE }, stateLeft{ GLFW_RELEASE };

bool mouseMoved=false;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	world->processScrollOffsets(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) {
			keys[key] = true;
		#if (DEBUG_LVL>=2)
			cout << key << ":PRESSED" << endl;
		#endif
		}
		else if (action == GLFW_RELEASE) {

			keys[key] = false;
		}
	}

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{



	double xOffset = xpos - lastX;
	double yOffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;


#if (DEBUG_LVL>=2)
	cout << xpos << "," << ypos << endl;
	cout << xOffset << "," << yOffset << endl;
#endif

	mouseMoved = true;
	world->processMouseOffsets(xOffset, yOffset);

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods ) {
	switch (button) {
	case GLFW_MOUSE_BUTTON_RIGHT:
		switch (action) {
		case GLFW_PRESS:
			stateRight = GLFW_PRESS;

			break;
		case GLFW_RELEASE:
			stateRight = GLFW_RELEASE;
			break;
		}
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		switch (action) {
		case GLFW_PRESS:
			stateMiddle = GLFW_PRESS;
			break;
		case GLFW_RELEASE:
			stateMiddle = GLFW_RELEASE;
			break;
		}
		break;
	case GLFW_MOUSE_BUTTON_LEFT:
		switch (action) {
		case GLFW_PRESS:
			stateLeft = GLFW_PRESS;
			break;
		case GLFW_RELEASE:
			stateLeft = GLFW_RELEASE;
			break;
		}
		break;
	}

}


int main(int argc, char **argv)
{

	GLFWwindow* window;
	if (!glfwInit())
		std::cout << "Failed to initialize GLFW" << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwMakeContextCurrent(window);
	
	glfwGetCursorPos(window, &lastX, &lastY);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
	}

	int err;
	if ( (err = glGetError()) != GL_NO_ERROR) {
		cout << "GlewInit has an error.. how interesting!"<< endl << GObject::returnErrorString(err);
		while ( (err = glGetError()) != GL_NO_ERROR)
			cout << endl << GObject::returnErrorString(err);
	}

#if _DEBUG
	cout << glGetString(GL_VENDOR) << endl << glGetString(GL_RENDERER) << endl;
	cout << glGetString(GL_VERSION) << endl << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	cout << "Max Texture Units:" << GL_MAX_TEXTURE_UNITS << endl;

#endif

	try {

		world = new World();
		

	
	while (!glfwWindowShouldClose(window))
	{
		GLdouble currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		world->processMouseButtons( stateLeft, stateMiddle, stateRight );
		world->processKeys ( keys, deltaTime );

		world->render( deltaTime );

		glFlush();

		glfwSwapBuffers(window);

	}


	}
	catch (GException e) {
		int i;
		cout << "Main()->" << e.what() << endl;
		cin >> i;
	}
	glfwTerminate();
	return 0;
}