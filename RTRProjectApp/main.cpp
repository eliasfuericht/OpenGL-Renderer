#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <Windows.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <SFML/Audio.hpp>

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
#include "Skybox.h"

#include <assimp/Importer.hpp>

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

std::vector<glm::vec3> testControlPoints = {
	glm::vec3(19.5, -0.6, 17)				   ,
glm::vec3(19.3418, -0.597721, 17.1212)		   ,
glm::vec3(17.9147, -0.577157, 18.2153)		   ,
glm::vec3(16.2959, -0.553832, 19.4561)		   ,
glm::vec3(16.0162, -0.537844, 19.5114)		   ,
glm::vec3(15.8594, -0.400165, 17.8287)		   ,
glm::vec3(15.694, -0.203628, 14.8403)		   ,
glm::vec3(15.7461, -0.0991198, 11.5604)		   ,
glm::vec3(15.4128, -0.0460886, 9.27749)		   ,
glm::vec3(14.1674, 0.0334897, 8.03158)		   ,
glm::vec3(11.3728, 0.268294, 7.06946)		   ,
glm::vec3(8.35249, 0.53219, 6.46561)		   ,
glm::vec3(5.9528, 0.787951, 6.10371)		   ,
glm::vec3(4.99258, 1.06807, 5.58027)		   ,
glm::vec3(4.63211, 1.23488, 4.88537)		   ,
glm::vec3(4.5628, 1.25948, 4.13546)			   ,
glm::vec3(5.22638, 1.27731, 3.68978)		   ,
glm::vec3(6.1181, 1.29157, 3.19336)			   ,
glm::vec3(6.82881, 1.29809, 2.53546)		   ,
glm::vec3(7.08591, 1.31884, 1.85537)		   ,
glm::vec3(7.05608, 1.35901, 0.73483)		   ,
glm::vec3(6.39788, 1.35067, -0.21606)		   ,
glm::vec3(5.36421, 1.33003, -1.17704)		   ,
glm::vec3(5.1351, 1.33992, -2.42194)		   ,
glm::vec3(3.54918, 1.32348, -3.46738)		   ,
glm::vec3(2.87568, 1.33361, -4.44419)		   ,
glm::vec3(2.76701, 1.44178, -5.63667)		   ,
glm::vec3(2.47016, 1.43646, -6.82742)		   ,
glm::vec3(2.0637, 1.34837, -7.71035)		   ,
glm::vec3(1.94551, 1.33202, -7.91199)		   ,
glm::vec3(1.35909, 1.36631, -8.28579)		   ,
glm::vec3(0.276196, 1.42032, -8.74194)		   ,
glm::vec3(-0.762164, 1.44227, -8.43364)		   ,
glm::vec3(-1.51647, 1.43329, -7.68134)		   ,
glm::vec3(-2.0107, 1.40096, -6.76266)		   ,
glm::vec3(-2.59054, 1.36358, -6.01834)		   ,
glm::vec3(-2.93153, 1.33584, -4.97027)		   ,
glm::vec3(-2.9368, 1.3234, -3.67107)		   ,
glm::vec3(-2.93578, 1.32058, -3.2745)		   ,
glm::vec3(-3.55258, 1.38226, -3.75509)		   ,
glm::vec3(-4.98679, 1.50513, -4.68103)		   ,
glm::vec3(-6.30466, 1.50233, -4.4528)		   ,
glm::vec3(-6.97657, 1.43954, -3.40217)		   ,
glm::vec3(-6.70778, 1.39252, -2.03872)		   ,
glm::vec3(-5.90654, 1.35119, -0.565722)		   ,
glm::vec3(-4.92745, 1.31807, 0.984055)		   ,
glm::vec3(-4.5405, 1.29971, 2.54898)		   ,
glm::vec3(-2.90876, 1.31189, 3.60728)		   ,
glm::vec3(-1.09819, 1.33399, 4.12053)		   ,
glm::vec3(1.09561, 1.33319, 4.24583)		   ,
glm::vec3(3.15841, 1.334, 4.62184)			   ,
glm::vec3(5.15947, 1.26621, 5.40086)		   ,
glm::vec3(7.50135, 0.995914, 6.49631)		   ,
glm::vec3(10.2399, 0.690808, 7.31399)
};

std::vector<glm::vec3> testTargetPoint = {
	glm::vec3(16.1251, -0.425445, 18.584),

	glm::vec3(16.4956, 0.121049, 8.80819),

	glm::vec3(10.9728, 0.459552, 7.047),

	glm::vec3(3.98437, 1.28137, 4.54027),
	glm::vec3(3.98437, 1.28137, 4.54027),

	//bunny
	glm::vec3(5.03575, 1.28534, 1.18781),
	glm::vec3(5.03575, 1.28534, 1.18781),
	glm::vec3(5.03575, 1.28534, 1.18781),
	glm::vec3(5.03575, 1.28534, 1.18781),
	
	//teapot
	glm::vec3(0.302491, 1.33864, -6.84412),
	glm::vec3(0.302491, 1.33864, -6.84412),
	glm::vec3(0.302491, 1.33864, -6.84412),
	
	//dragon
	glm::vec3(-5.08298, 1.4183, -2.57099),
	glm::vec3(-5.08298, 1.4183, -2.57099),
	glm::vec3(-5.08298, 1.33864, -2.57099),
	glm::vec3(-5.08298, 1.28534, -2.57099),
	glm::vec3(-5.08298, 1.28534, -2.57099),

	glm::vec3(6.86635, 1.09922, 6.03722),

	glm::vec3(14.0616, 2.46968, 7.42632),
	glm::vec3(14.0616, 2.46968, 7.42632)
};

quadratic_uniform_b_spline testCameraPath;
quadratic_uniform_b_spline testTargetPath;

Material shinyMaterial;
Material dullMaterial;
Material lightMaterial;

Model debugCube;
Model scene;
Model sculptures;
Model lightSphere;

sf::SoundBuffer buffer;
sf::Sound sound;

Texture dirtTexture;

DirectionalLight mainDirectionalLight;
PointLight pointLights[MAX_POINT_LIGHTS];

Skybox skybox;

double deltaTime = 0.0f;
double elapsedTime = 0.0f;
double animationDuration = 120.0f;
double lastTime = 0.0f;
double t = 0.0f;

//Renderpass
static const char* vShader = "Shaders/vertex.glsl";
static const char* fShader = "Shaders/fragment.glsl";

//Directional Shadowmapping
static const char* dShadowVertShader = "Shaders/dShadowMapVertex.glsl";
static const char* dShadowFragShader = "Shaders/dShadowMapFragment.glsl";

//Omnidirectional Shadowmapping
static const char* oShadowVertShader = "Shaders/oShadowMapVertex.glsl";
static const char* oShadowGeoShader = "Shaders/oShadowMapGeometry.glsl";
static const char* oShadowFragShader = "Shaders/oShadowMapFragment.glsl";

//Skybox
static const char* skyBoxVertShader = "Shaders/skyBoxVertex.glsl";
static const char* skyBoxFragShader = "Shaders/skyBoxFragment.glsl";

// setting up GLuints for uniform locations for later use
GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0, uniformLightSpace = 0, uniformDShadowMap = 0, uniformSkybox = 0, uniformLightPos = 0, uniformLightFarPlane = 0;
GLuint* uniformOShadowMatrices = 0;

#ifdef _WIN32
// Use discrete GPU by default.
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

void renderRealScene() {

	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

	glm::mat4 model(1.0f);

	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, -1.25f, 0.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	scene.RenderModel();

	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

	sculptures.RenderModel();
}

int main()
{
	mainWindow = Window(1920, 1080, false);
	mainWindow.Initialise();

	Shader* sDShadowPass = new Shader();
	Shader* sOShadowPass = new Shader();
	Shader* sRenderPass = new Shader();
	Shader* sSkybox = new Shader();

	sDShadowPass->CreateFromFiles(dShadowVertShader, "", dShadowFragShader);
	sOShadowPass->CreateFromFiles(oShadowVertShader, oShadowGeoShader, oShadowFragShader);
	sRenderPass->CreateFromFiles(vShader, "", fShader);
	sSkybox->CreateFromFiles(skyBoxVertShader, "",  skyBoxFragShader);

	// camera with correct startposition for final scene
	camera = Camera(glm::vec3(19.5f, -0.60f, 17.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.05f);

	testCameraPath = quadratic_uniform_b_spline(testControlPoints);
	testTargetPath = quadratic_uniform_b_spline(testTargetPoint);

	shinyMaterial = Material(8.0f, 256);
	dullMaterial = Material(0.3f, 4);

	lightMaterial = Material(20.0f, 128);

	buffer = sf::SoundBuffer();
	buffer.loadFromFile("Audio/ambientMusic.mp3");

	debugCube = Model();
	debugCube.LoadModel("Models/cube.obj");

	scene = Model();
	scene.LoadModel("Models/scene.obj");
	
	sculptures = Model();
	sculptures.LoadModel("Models/sculptures.obj");

	lightSphere = Model();
	lightSphere.LoadModel("Models/sphere.obj");
	
	sound = sf::Sound();
	sound.setBuffer(buffer);
	sound.play();

	printf("Initial loading took: %f seconds\n", glfwGetTime());

	mainDirectionalLight = DirectionalLight(80.0f/255.0f, 104.0f/255.0f, 134.0f/255.0f,
											0.5f, 0.0f,
											0.82f, 0.96f, 1.61f, 
											8192, 8192);

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
								0.5f, 0.1f,
								0.0f, 2.5f, 0.0f,
								10.0f, -1.5f, 0.1f,
								2048, 2048);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
								0.5f, 0.1f,
								1.0f, 2.5f, 1.0f,
								10.0f, -1.5f, 0.1f,
								2048, 2048);
	pointLightCount++;

	skybox = Skybox("skybox1");

	// calculating prespective projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 200.0f);

	double lastFrame = 0.0f;
	int frameCount = 0;
	int fps = 0;
	glfwSetTime(0.0f);
	double animationTime = 0.0f;

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{

		double now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		frameCount++;
		if (now - lastFrame >= 1.0)
		{
			fps = frameCount;
			frameCount = 0;
			lastFrame = now;
			std::string FPS = std::to_string(fps);
			std::string newTitle = "Night at the museum - " + FPS + "FPS";
			glfwSetWindowTitle(mainWindow.getGLFWWindow(), newTitle.c_str());
		}

		// Get + Handle User Input
		glfwPollEvents();

		// camera animation
		//set t to control duration of animation
		animationTime += deltaTime;
		t = animationTime / animationDuration;
		t = glm::clamp(t, 0.0, 1.0);

		glm::vec3 newCameraPos = testCameraPath.value_at(t);

		camera.setCameraPosition(newCameraPos);

		glm::vec3 target = testTargetPath.value_at(t);

		camera.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 viewMatrix = glm::lookAt(camera.getCameraPosition(), target, camera.getCameraUp());

		// Extract the direction vector from the view matrix
		glm::vec3 cameraDirection = glm::normalize(glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]));

		camera.setCameraDirection(-cameraDirection);

		pointLights[0].SetLightPosition(testCameraPath.value_at(t + 0.02f));
		pointLights[1].SetLightPosition(testCameraPath.value_at(t - 0.02f));

		//DIRECTIONAL SHADOWPASS
		// sets shaderprogram at sDShadowPass as shaderprogram to use
		// sDShadowPass = shadowpass
		sDShadowPass->UseShader();
		
		uniformLightSpace = sDShadowPass->GetLightSpaceMatrixLocation();
		uniformModel = sDShadowPass->GetModelLocation();

		mainDirectionalLight.WriteShadowMap(uniformLightSpace);
		
		renderRealScene();

		mainDirectionalLight.UnbindShadowMap();

		//OMNIDIRECTIONAL SHADOWPASS
		sOShadowPass->UseShader();
		
		uniformModel = sOShadowPass->GetModelLocation();
		uniformOShadowMatrices = sOShadowPass->GetOShadowMatrices();
		uniformLightPos = sOShadowPass->GetLightPosLocation();
		uniformLightFarPlane = sOShadowPass->GetLightFarPlane();

		for (size_t i = 0; i < pointLightCount; i++) {
			pointLights[i].WriteShadowMap(uniformOShadowMatrices, uniformLightPos, uniformLightFarPlane);

			renderRealScene();

			pointLights[i].UnbindShadowMap();
		}

		//RENDERPASS
		// sets shaderprogram at sRenderPass as shaderprogram to use
		// = renderpass
		sRenderPass->UseShader();

		// retreive uniform locations (ID) from shader membervariables
		// and stores them in local varibale for passing projection, model and view matrices to shader
		uniformModel = sRenderPass->GetModelLocation();
		uniformProjection = sRenderPass->GetProjectionLocation();
		uniformView = sRenderPass->GetViewLocation();
		uniformEyePosition = sRenderPass->GetEyePositionLocation();
		uniformSpecularIntensity = sRenderPass->GetSpecularIntensityLocation();
		uniformShininess = sRenderPass->GetShininessLocation();
		uniformLightSpace = sRenderPass->GetLightSpaceMatrixLocation();
		uniformSkybox = sRenderPass->GetSkyBoxLocation();
		uniformLightFarPlane = sRenderPass->GetLightFarPlane();

		// Clear window and reset scene
		glViewport(0, 0, mainWindow.getBufferWidth(), mainWindow.getBufferHeight());
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// sends data about the lights from CPU to the (fragement)shader at corresponding locations
		sRenderPass->SetDirectionalLight(&mainDirectionalLight);
		sRenderPass->SetPointLights(pointLights, pointLightCount, 3, 0);

		sRenderPass->SetTexture(1);

		mainDirectionalLight.ReadShadowMap();
		sRenderPass->SetDirectionalShadowMap(2);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniformMatrix4fv(uniformLightSpace, 1, GL_FALSE, glm::value_ptr(mainDirectionalLight.GetLightSpaceMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		glUniform1f(uniformLightFarPlane, pointLights[0].GetFarPlane());

		//comparable to UseLight() in DirectionalLight.cpp but for Material
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		renderRealScene();

		lightMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		glm::mat4 model(1.0f);

		model = glm::mat4(1.0f);

		model = glm::translate(model,pointLights[0].GetLightPosition());

		model = glm::scale(model,glm::vec3(0.01,0.01,0.01));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		lightSphere.RenderModel();

		model = glm::mat4(1.0f);

		model = glm::translate(model, pointLights[1].GetLightPosition());

		model = glm::scale(model,glm::vec3(0.01,0.01,0.01));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		lightSphere.RenderModel();

		//SKYBOX PASS
		sSkybox->UseShader();

		uniformProjection = sSkybox->GetProjectionLocation();
		uniformView = sSkybox->GetViewLocation();
		uniformModel = sSkybox->GetModelLocation();
		
		pointLights[0].ReadShadowMap();

		sSkybox->SetOmniDirectionalShadowMap(2);

		skybox.RenderSkybox(uniformProjection, projection, uniformView, camera.calculateViewMatrix(), uniformModel);
		
		debugCube.RenderModel(); //cube for skybox

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	glfwDestroyWindow(mainWindow.getGLFWWindow());
	glfwTerminate();

	return 0;
}