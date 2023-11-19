#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <numeric>
#include <cstdlib>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"
#include "quadratic_uniform_b_spline.h"

#include <assimp/Importer.hpp>

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

std::vector<glm::vec3> controlPointsComplete = { 
	//forest path
	glm::vec3(19.50, -0.60, 17.00),
	glm::vec3(19.50, -0.60, 17.00),
	glm::vec3(16.03, -0.55, 19.66),
	glm::vec3(16.03, -0.55, 19.66),
	glm::vec3(15.62, -0.19, 15.26),
	glm::vec3(15.84, -0.06, 9.98),
	glm::vec3(14.71, -0.02, 7.99),
	glm::vec3(9.73, 0.41, 6.60),
	glm::vec3(5.44, 0.79, 6.14),
	
	//bunny
	glm::vec3(4.72, 1.24, 5.25),
	glm::vec3(4.37, 1.26, 3.97),
	glm::vec3(5.85, 1.29, 3.49),
	glm::vec3(7.06, 1.30, 2.31),
	glm::vec3(7.06, 1.30, 2.31),
	glm::vec3(7.17, 1.38, 0.38),
	glm::vec3(5.59, 1.32, -0.84),
	glm::vec3(3.98, 1.35, -2.88),
	
	//teapot
	glm::vec3(2.77, 1.29, -4.22),
	glm::vec3(2.80, 1.51, -6.13),
	glm::vec3(1.98, 1.33, -7.89),
	glm::vec3(1.98, 1.33, -7.89),
	glm::vec3(0.27, 1.43, -8.98),
	glm::vec3(-1.32, 1.45, -8.16),
	glm::vec3(-2.01, 1.40, -6.65),
	
	//dragon
	glm::vec3(-2.97, 1.34, -5.63),
	glm::vec3(-2.93, 1.32, -3.27),
	glm::vec3(-2.93, 1.32, -3.27),
	glm::vec3(-5.33, 1.56, -5.14),
	glm::vec3(-7.19, 1.45, -3.83),
	glm::vec3(-6.68, 1.38, -1.68),
	
	//end
	glm::vec3(-5.07, 1.32, 0.64),
	glm::vec3(-3.81, 1.29, 3.15),
	glm::vec3(-1.51, 1.34, 4.21),
	glm::vec3(2.05, 1.33, 4.22),
	glm::vec3(4.92, 1.34, 5.24),
	glm::vec3(8.66, 0.84, 7.09),
	glm::vec3(12.37, 0.40, 7.79),
	glm::vec3(15.00, 0.39, 9.00),
	glm::vec3(14.20, 0.51, 8.65)
};

std::vector<glm::vec3> targetPoints = {
	//museum
	glm::vec3(0.0f, 0.0, 3.0f),
	glm::vec3(3.0f, 0.0, -3.0f),
	//bunny
	glm::vec3(5.08, 1.30, 1.20),
	glm::vec3(5.08, 1.30, 1.20),
	glm::vec3(5.08, 1.30, 1.20),
	//teapot
	glm::vec3(5.18, 1.54, -1.12),
	glm::vec3(2.92, 1.35, -4.29),
	glm::vec3(2.92, 1.35, -4.29),
	glm::vec3(0.26, 1.33, -7.20),
	glm::vec3(0.26, 1.33, -7.20),
	glm::vec3(0.26, 1.33, -7.20),
	glm::vec3(0.26, 1.33, -7.20),
	glm::vec3(1.0, 1.35, -5.29),
	
	//dragon
	glm::vec3(-4.89, 1.30, -2.62),
	glm::vec3(-5.45, 1.25, -2.94),
	glm::vec3(-5.45, 1.25, -2.94),
	glm::vec3(-5.45, 1.25, -2.94),
	glm::vec3(-4.89, 1.30, -2.62),
	glm::vec3(-4.89, 1.30, -2.62),
	//outwards
	glm::vec3(-3.33, 1.40, 3.31),
	glm::vec3(4.42, 1.38, 4.39),
	glm::vec3(13.71, 0.94, 8.88),
	glm::vec3(13.71, 0.94, 8.88),
	glm::vec3(13.71, 0.94, 8.88),
	glm::vec3(3.48, 1.37, 2.68)
};

quadratic_uniform_b_spline cameraPath;
quadratic_uniform_b_spline targetPath;

Material shinyMaterial;
Material dullMaterial;

Model debugPlane;
Model debugCube;
Model scene;

Texture dirtTexture;

DirectionalLight mainDirectionalLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

double deltaTime = 0.0f;
double elapsedTime = 0.0f;
double animationDuration = 120.0f;
double lastTime = 0.0f;
double t = 0.0f; //Bezier parameter t

// Vertex Shader
static const char* vShader = "Shaders/vertex.glsl";

// Fragment Shader
static const char* fShader = "Shaders/fragment.glsl";

void CreateShaders()
{
	Shader *shader1 = new Shader();
	// reads, compiles and sets shader as shaderprogram to use!
	shader1->CreateFromFiles(vShader, fShader);
	//pushes into shaderList vector (for later use of multiple shaders)
	shaderList.push_back(*shader1);
}

int main() 
{
	mainWindow = Window(1920, 1080);
	mainWindow.Initialise();

	CreateShaders();

	// setting up basic camera
	// camera with correct startposition for final scene
	camera = Camera(glm::vec3(19.5f, -0.60f, 17.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.05f);

	cameraPath = quadratic_uniform_b_spline(controlPointsComplete);
	targetPath = quadratic_uniform_b_spline(targetPoints);

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();

	printf("loading models...\n");

	debugPlane = Model();
	debugPlane.LoadModel("Models/plane.obj");

	debugCube = Model();
	debugCube.LoadModel("Models/cube.obj");

	scene = Model();
	//scene.LoadModel("Models/scene.obj");


	printf("Initial loading took: %f seconds\n", glfwGetTime());

	// setting up lights (position, color, ambientIntensity, diffuseIntensity, direction, edge)
	// and incrementing the corresponding lightCount
	mainDirectionalLight = DirectionalLight(1.0f, 1.0f, 1.0f,
								0.25f, 0.1f,
								0.0f, 0.0f, -1.0f);

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
								1.0f, 0.1f,
								0.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//flashlight
	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
						0.1f, 0.5f,
						0.0f, 0.0f, 0.0f,
						0.0f, -1.0f, 0.0f,
						0.8f, 0.0f, 0.0f,
						10.0f);

	spotLightCount++;

	// setting up GLuints for uniform locations for later use
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	
	// calculating projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//print OpenGL Version
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "OpenGL version: " << version << std::endl;

	double lastFrame = 0.0f;
	int frameCount = 0;
	int fps = 0;
	std::vector<int> fpsList;
	glfwSetTime(0.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{   
		double now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();	
		
		//setting up camera animation
		if (mainWindow.getAnimationBool()) {
			//set t to control duration of animation
			t = now / animationDuration;
			t = glm::clamp(t, 0.0, 1.0);

			camera.setCameraPosition(cameraPath.value_at(t));

			glm::vec3 target = targetPath.value_at(t);

			glm::mat4 viewMatrix = glm::lookAt(camera.getCameraPosition(), target, glm::vec3(0.0f, 1.0f, 0.0f));

			// Extract the direction vector from the view matrix
			glm::vec3 cameraDirection = glm::normalize(glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]));

			camera.setCameraDirection(-cameraDirection);
		}
		// if animation is toggled off -> use WASD and mouse to navigate
		else {
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			camera.keyControl(mainWindow.getKeys(), deltaTime);
		}

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// sets shaderprogram at shaderList[0] as shaderprogram to use
		shaderList[0].UseShader();

		// retreive uniform locations (ID) from shader membervariables
		// and stores them in local varibale for passing projection, model and view matrices to shader
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		//Flashlight
		// copies camera position and lowers y value by 0.3f (so flashlight feels like it's in hand)
		// SetFlash() sets the direction of the light to always face the same direction as the camera
		glm::vec3 flashLightPosition = camera.getCameraPosition();
		spotLights[0].SetFlash(flashLightPosition, camera.getCameraDirection());

		// sends data about the lights from CPU to the (fragement)shader at corresponding locations
		shaderList[0].SetDirectionalLight(&mainDirectionalLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		// sends transformationmatrix to (vertex)shader to corresponding locations
		// = uniform mat4 projection;
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		// = uniform mat4 view;
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// sends camera position to (fragment)shader to corresponding locations
		// = uniform vec3 eyePosition;
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0f);	

		//comparable to UseLight() in DirectionalLight.cpp but for Material
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));

		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		debugPlane.RenderModel();


		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, -1.25f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		debugCube.RenderModel();
		//scene.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();

		frameCount++;
		if (now - lastFrame >= 1.0)
		{
			fps = frameCount;
			fpsList.push_back(fps);
			frameCount = 0;
			lastFrame = now;
		}
		printf("\rCurrent FPS: %d", fps);
	}

	int averageFPS = std::accumulate(fpsList.begin(), fpsList.end(), 0) / fpsList.size();
	printf("\nAverage FPS: %d\n", averageFPS);

	return 0;
}