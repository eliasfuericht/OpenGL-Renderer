#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	fullscreen = false;

	for(size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	
	xChange = 0.0f;
	yChange = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight, bool fs)
{
	width = windowWidth;
	height = windowHeight;
	fullscreen = fs;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	
	xChange = 0.0f;
	yChange = 0.0f;
}

int Window::Initialise()
{
	if (!glfwInit())
	{
		printf("Error Initialising GLFW");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW Windows Properties
	// OpenGL version, profile and compatability
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create window
	if (fullscreen) {
		mainWindow = glfwCreateWindow(width, height, "Night at the museum", glfwGetPrimaryMonitor(), NULL);
	}
	else {
		mainWindow = glfwCreateWindow(width, height, "Night at the museum", NULL, NULL);
	}
	
	if (!mainWindow)
	{
		printf("Error creating GLFW window!");
		glfwTerminate();
		return 1;
	}

	// Get Framebuffer information and set it to variables
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the current context
	glfwMakeContextCurrent(mainWindow);

	// Handle Key + Mouse Input 
	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Allow modern extensions 
	glewExperimental = GL_TRUE;

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Enable Z buffer
	glEnable(GL_DEPTH_TEST);

	// Create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	// Sets handleKeys as callback function for keys and mouse
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange()
{
	GLfloat changeValueX = xChange;
	xChange = 0.0f;
	return changeValueX;
}

GLfloat Window::getYChange()
{
	GLfloat changeValueY = yChange;
	yChange = 0.0f;
	return changeValueY;
}

// handleKeys function has to be the exact signature as below for it to work (see -> GLFWkeyfun)
void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		theWindow->animation = !theWindow->animation;
	}

	// sets key to true if pressed and false if released
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

// function has to be the exact signature as below for it to work (see -> GLFWcursorposfun)
void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
