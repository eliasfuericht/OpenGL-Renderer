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
#include "BezierCurve.h"

#include <assimp/Importer.hpp>

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Material shinyMaterial;
Material dullMaterial;

Model debugOBJ;
Model scene;

Texture dirtTexture;

BezierCurve cameraPath;

std::vector<glm::vec3> controlPoints = { glm::vec3(19.5f, -0.60f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f) };

DirectionalLight mainDirectionalLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat elapsedTime = 0.0f;
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

std::vector<glm::vec3> readCoordinatesFromFile(const std::string& filePath) {
	std::ifstream file(filePath);
	std::vector<glm::vec3> coordinates;
	std::string line;

	while (std::getline(file, line)) {
		glm::vec3 vec;
		sscanf(line.c_str(), "%f, %f, %f,", &vec.x, &vec.y, &vec.z);
		vec *= 10;
		coordinates.push_back(vec);
	}

	return coordinates;
}

int main() 
{
	mainWindow = Window(1920, 1080);
	mainWindow.Initialise();

	CreateShaders();

	// setting up basic camera
	camera = Camera(glm::vec3(19.5f, -0.60f, 17.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.05f);

	cameraPath = BezierCurve(controlPoints);

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	printf("loading models...\n");

	debugOBJ = Model();
	debugOBJ.LoadModel("Models/tree.obj");

	scene = Model();
	scene.LoadModel("Models/sceneWODRagon.obj");


	printf("Initial loading took: %f seconds\n", glfwGetTime());

	// setting up lights (position, color, ambientIntensity, diffuseIntensity, direction, edge)
	// and incrementing the corresponding lightCount
	mainDirectionalLight = DirectionalLight(255.0f/255.0f, 211.0f / 255.0f, 168.0f/ 255.0f,
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
		printf("\rCurrent FPS: %d", fps);
		elapsedTime = now - startTime;
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

		glm::vec3 nextPosition;
		//setting up camera animation
		t = elapsedTime / 60.0f;
		t = glm::clamp(t, 0.0f, 1.0f);
		nextPosition = cameraPath.value_at(t);
		camera.updatePosition(nextPosition);
		

		// Handle camera movement
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		camera.keyControl(mainWindow.getKeys(), deltaTime);

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
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f + glm::sin(glfwGetTime()*2.0f) * 0.1f;
	
		// SetFlash() sets the direction of the light to always face the same direction as the camera
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

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

		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		scene.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	int averageFPS = 0;
	averageFPS = std::accumulate(fpsList.begin(), fpsList.end(), 0) / fpsList.size();
	printf("\nAverage FPS: %d\n", averageFPS);

	/*glm::vec3 currentCameraPos = camera.getCameraPosition();
	double positionX = currentCameraPos[0];
	double positionY = currentCameraPos[1];
	double positionZ = currentCameraPos[2];
	printf("\nX: %lf\n", positionX);
	printf("\nY: %lf\n", positionY);
	printf("\nZ: %lf\n", positionZ);*/

	return 0;
}