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
#include "bezier_curve.h"
#include "quadratic_uniform_b_spline.h"
#include "BezierCurve.h"

#include <assimp/Importer.hpp>

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

std::vector<glm::vec3> controlPoints = { 
	//start
	glm::vec3(19.50, -0.60, 17.00),
	glm::vec3(19.50, -0.60, 17.00),
	glm::vec3(19.50, -0.60, 17.00),
	glm::vec3(16.03, -0.55, 19.66),
	glm::vec3(15.62, -0.19, 15.26),
	glm::vec3(15.84, -0.06, 9.98),
	glm::vec3(15.84, -0.06, 9.98),
	glm::vec3(14.71, -0.02, 7.99),
	glm::vec3(9.73, 0.41, 6.60),
	glm::vec3(5.44, 0.79, 6.14),
	//bunny
	glm::vec3(5.44, 0.79, 6.14),
	glm::vec3(3.20, 0.80, 3.99),
	glm::vec3(3.29, 0.96, 2.51),
	glm::vec3(5.72, 1.10, 2.75),
	glm::vec3(5.53, 1.06, 2.90),
	glm::vec3(6.29, 1.07, 2.52),
	glm::vec3(6.29, 1.07, 2.52),
	glm::vec3(6.95, 1.07, 2.03),
	glm::vec3(6.95, 1.07, 2.03),
	glm::vec3(6.95, 1.07, 2.03),
	glm::vec3(7.31, 1.07, 0.52),
	glm::vec3(7.31, 1.07, 0.52),
	glm::vec3(7.31, 1.07, 0.52),
	glm::vec3(6.68, 1.08, -0.65),
	glm::vec3(5.29, 1.04, -1.04),
	glm::vec3(4.20, 0.90, -2.90),
	glm::vec3(4.44, 0.85, -3.17),
	//teapot
	glm::vec3(4.20, 0.90, -2.90),
	glm::vec3(4.44, 0.85, -3.17),
	glm::vec3(3.42, 0.92, -3.44),
	glm::vec3(2.16, 0.97, -4.87),
	glm::vec3(2.16, 0.97, -4.87),
	glm::vec3(2.16, 0.97, -4.87),
	glm::vec3(0.42, 0.98, -4.60),
	glm::vec3(-1.80, 1.18, -7.18),
	glm::vec3(-0.91, 1.15, -8.70),
	glm::vec3(-1.62, 1.08, -7.39),
	glm::vec3(-1.62, 1.08, -7.39),
	glm::vec3(-1.62, 1.08, -7.39),
	glm::vec3(-2.24, 1.04, -6.22),
	glm::vec3(-2.24, 1.04, -6.22),

	//dragon
	glm::vec3(-3.10, 1.10, -4.90),
	glm::vec3(-3.47, 0.96, -3.05),
	glm::vec3(-3.56, 0.95, -2.15),
	glm::vec3(-3.56, 0.95, -2.15),
	glm::vec3(-4.63, 0.99, -0.83),
	glm::vec3(-4.63, 0.99, -0.83),
	glm::vec3(-4.63, 0.99, -0.83),
	glm::vec3(-6.30, 1.05, -1.12),
	glm::vec3(-6.30, 1.05, -1.12),
	glm::vec3(-6.30, 1.05, -1.12),
	glm::vec3(-6.81, 0.98, -2.85),
	glm::vec3(-6.78, 0.90, -0.70),

	//endpath
	glm::vec3(-6.78, 0.90, -0.70),
	glm::vec3(-6.62, 0.92, 0.47),
	glm::vec3(-4.35, 1.07, 2.12),
	glm::vec3(-3.83, 1.15, 3.82),
	glm::vec3(-1.12, 1.19, 4.27),
	glm::vec3(2.07, 1.03, 2.97),
	glm::vec3(4.70, 1.12, 4.66),
	glm::vec3(7.36, 0.0, 6.52),
	glm::vec3(11.74, 0.0, 7.66),
	glm::vec3(11.0, 0.0, 7.35)
};

std::vector<glm::vec3> debugPoints = {
	glm::vec3(0.0f, 0.0, 5.0f),
	glm::vec3(-5.0f, 5.0, 0.0f),
	glm::vec3(0.0f, 0.0, -5.0f),
	glm::vec3(5.0f, 0.0, 0.0f),
	glm::vec3(-5.0f, 0.0, 5.0f),
	glm::vec3(-2.0f, 0.0, 5.0f),
};

std::vector<glm::vec3> debugTargetPoints = {
	glm::vec3(0.0f, 0.0, 3.0f),
	glm::vec3(3.0f, 0.0, -3.0f),
	glm::vec3(-3.0f, 0.0, -3.0f),
	glm::vec3(-3.0f, 0.0, 3.0f),
	glm::vec3(-5.0f, 0.0, 5.0f),
	glm::vec3(-2.0f, 0.0, 4.0f),
};

bezier_curve debugCurve;
bezier_curve debugTargetCurve;

quadratic_uniform_b_spline debugSpline;
quadratic_uniform_b_spline debugTargetSpline;

Material shinyMaterial;
Material dullMaterial;

Model debugPlane;
Model debugCube;
Model scene;

Texture dirtTexture;

//BezierCurve cameraPath;
BezierCurve cameraPath1;
BezierCurve cameraPath2;
BezierCurve cameraPath3;

std::vector<glm::vec3> controlPoints1 = { //forest path
	glm::vec3(19.50, -0.60, 17.00),
	glm::vec3(19.50, -0.60, 17.00),
	glm::vec3(19.50, -0.60, 17.00),
	glm::vec3(16.03, -0.55, 19.66),
	glm::vec3(15.62, -0.19, 15.26),
	glm::vec3(15.84, -0.06, 9.98),
	glm::vec3(15.84, -0.06, 9.98),
	glm::vec3(14.71, -0.02, 7.99),
	glm::vec3(9.73, 0.41, 6.60),
	glm::vec3(5.44, 0.79, 6.14),
	//bunny
	glm::vec3(5.44, 0.79, 6.14),
	glm::vec3(3.20, 0.80, 3.99),
	glm::vec3(3.29, 0.96, 2.51),
	glm::vec3(5.72, 1.10, 2.75),
	glm::vec3(5.53, 1.06, 2.90),
	glm::vec3(6.29, 1.07, 2.52),
	glm::vec3(6.29, 1.07, 2.52),
	glm::vec3(6.95, 1.07, 2.03),
	glm::vec3(6.95, 1.07, 2.03),
	glm::vec3(6.95, 1.07, 2.03),
	glm::vec3(7.31, 1.07, 0.52),
	glm::vec3(7.31, 1.07, 0.52),
	glm::vec3(7.31, 1.07, 0.52),
	glm::vec3(6.68, 1.08, -0.65),
	glm::vec3(5.29, 1.04, -1.04),
	glm::vec3(4.20, 0.90, -2.90),
	glm::vec3(4.44, 0.85, -3.17),
};

std::vector<glm::vec3> controlPoints2 = { //teapot
glm::vec3(4.20, 0.90, -2.90),
glm::vec3(4.44, 0.85, -3.17),
glm::vec3(3.42, 0.92, -3.44),
glm::vec3(2.16, 0.97, -4.87),
glm::vec3(2.16, 0.97, -4.87),
glm::vec3(2.16, 0.97, -4.87),
glm::vec3(0.42, 0.98, -4.60),
glm::vec3(-1.80, 1.18, -7.18),
glm::vec3(-0.91, 1.15, -8.70),
glm::vec3(-1.62, 1.08, -7.39),
glm::vec3(-1.62, 1.08, -7.39),
glm::vec3(-1.62, 1.08, -7.39),
glm::vec3(-2.24, 1.04, -6.22),
glm::vec3(-2.24, 1.04, -6.22),


//dragon
glm::vec3(-3.10, 1.10, -4.90),
glm::vec3(-3.47, 0.96, -3.05),
glm::vec3(-3.56, 0.95, -2.15),
glm::vec3(-3.56, 0.95, -2.15),
glm::vec3(-4.63, 0.99, -0.83),
glm::vec3(-4.63, 0.99, -0.83),
glm::vec3(-4.63, 0.99, -0.83),
glm::vec3(-6.30, 1.05, -1.12),
glm::vec3(-6.30, 1.05, -1.12),
glm::vec3(-6.30, 1.05, -1.12),
glm::vec3(-6.81, 0.98, -2.85),
glm::vec3(-6.78, 0.90, -0.70)
};


std::vector<glm::vec3> controlPoints3 = { //endpath
glm::vec3(-6.78, 0.90, -0.70),
glm::vec3(-6.62, 0.92, 0.47),
glm::vec3(-4.35, 1.07, 2.12),
glm::vec3(-3.83, 1.15, 3.82),
glm::vec3(-1.12, 1.19, 4.27),
glm::vec3(2.07, 1.03, 2.97),
glm::vec3(4.70, 1.12, 4.66),
glm::vec3(7.36, 0.0, 6.52),
glm::vec3(11.74, 0.0, 7.66),
glm::vec3(11.0, 0.0, 7.35)
};



DirectionalLight mainDirectionalLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat elapsedTime = 0.0f;
GLfloat animationDuration = 180.0f;
GLfloat lastTime = 0.0f;
GLfloat t = 0.0f; //Bezier parameter t

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
	//camera = Camera(glm::vec3(19.5f, -0.60f, 17.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.05f);
	// debug camera
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.05f);

	debugCurve = bezier_curve(debugPoints);
	debugTargetCurve = bezier_curve(debugTargetPoints);

	debugSpline = quadratic_uniform_b_spline(controlPoints);
	debugTargetSpline = quadratic_uniform_b_spline(debugTargetPoints);

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
	scene.LoadModel("Models/scene.obj");


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
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
								0.1f, 0.1f,
								-4.0f, 2.0f, 0.0f,
								0.3f, 0.1f, 0.1f);
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
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
		-100.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
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
	//glfwSetTime(0.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{   
		static double startTime = glfwGetTime();
		double now = glfwGetTime();
		//printf("\rCurrent FPS: %d", fps);
		deltaTime = now - lastTime;
		lastTime = now;

		frameCount++;
		if (now - lastFrame >= 1.0)
		{
			fps = frameCount;
			fpsList.push_back(fps);
			frameCount = 0;
			lastFrame = now;
		}


		//pointLights[0].SetLightPosition(glm::vec3(camera.getCameraPosition().x+2.0f, camera.getCameraPosition().y, camera.getCameraPosition().z));

		// Get + Handle User Input
		glfwPollEvents();

		if (camera.printposition) {
			glm::vec3 currentCameraPos = camera.getCameraPosition();
			double positionX = currentCameraPos[0];
			double positionY = currentCameraPos[1];
			double positionZ = currentCameraPos[2];
			printf("\nglm::vec3( %.2f, %.2f, %.2f)\n", positionX, positionY, positionZ);
		}
		

		// Handle camera movement
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		camera.keyControl(mainWindow.getKeys(), deltaTime);

		//setting up camera animation

		if (camera.animationOn) {
			glm::vec3 nextPosition;
			glm::vec3 tangent;
			glm::vec3 nextControlPoint;

			elapsedTime = now - startTime;
			t = elapsedTime / animationDuration;
			t = t * 5;
			t = glm::clamp(t, 0.0f, 3.0f);

			if (t <= 1.0) {
				nextPosition = cameraPath1.value_at(t);
				tangent = cameraPath1.slope_at(t);
				nextControlPoint = cameraPath1.value_at(t + 0.1);
			} else if (t <= 2.0) {
				float t2 = t - 1.0;
				nextPosition = cameraPath2.value_at(t2);
				tangent = cameraPath2.slope_at(t2);
				nextControlPoint = cameraPath2.value_at(t2 + 0.1);
			} else if (t <= 3.0) {
				float t3 = t - 2.0;
				nextPosition = cameraPath3.value_at(t3);
				tangent = cameraPath3.slope_at(t3);
				nextControlPoint = cameraPath3.value_at(t3 + 0.1);
			}

			camera.updatePosition(nextPosition);
			camera.updateOrientation(tangent, nextControlPoint);
		}

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// sets shaderprogram at shaderList[0] as shaderprogram to use
		shaderList[0].UseShader();

		camera.setCameraPosition(debugSpline.value_at(glm::clamp(now * 0.01,0.0,1.0)));

		glm::vec3 target = debugTargetSpline.value_at(glm::clamp(now * 0.01+0.1, 0.0, 1.0));
		
		glm::mat4 viewMatrix = glm::lookAt(camera.getCameraPosition(), target, glm::vec3(0.0f, 1.0f, 0.0f));

		// Extract the direction vector from the view matrix
		glm::vec3 cameraDirection = glm::normalize(glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]));
		
		camera.setCameraDirection(-cameraDirection);

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
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f + glm::sin(glfwGetTime()*2.0f) * 0.1f;
	
		// SetFlash() sets the direction of the light to always face the same direction as the camera
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//target = debugTargetCurve.value_at(glm::clamp(now * 0.1, 0.0, 1.0));

		//spotLights[0].SetLightPosition(target);

		// sends data about the lights from CPU to the (fragement)shader to corresponding locations
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

		//comparable to UseLight() in DirectionalLight.cpp (but for Material)
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));

		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//debugPlane.RenderModel();


		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, -1.25f, 0.0f));

		//model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//dirtTexture.UseTexture();

		//debugCube.RenderModel();
		scene.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	int averageFPS = 0;
	averageFPS = std::accumulate(fpsList.begin(), fpsList.end(), 0) / fpsList.size();
	printf("\nAverage FPS: %d\n", averageFPS);

	glm::vec3 currentCameraPos = camera.getCameraPosition();
	double positionX = currentCameraPos[0];
	double positionY = currentCameraPos[1];
	double positionZ = currentCameraPos[2];
	printf("\nglm::vec3( %.2f, %.2f, %.2f)\n", positionX, positionY, positionZ);


	return 0;
}