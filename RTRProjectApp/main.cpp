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

Material dullMaterial;

Model debugCube;
Model debugPlane;
Model lightSphere;

Mesh cloth;

Texture dirtTexture;
Texture defaultTexture;

DirectionalLight mainDirectionalLight;

double deltaTime = 0.0f;
double elapsedTime = 0.0f;
double animationDuration = 120.0f;
double lastTime = 0.0f;
double t = 0.0f;

//Renderpass
static const char* vShader = "Shaders/vertex.glsl";
static const char* fShader = "Shaders/fragment.glsl";

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

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

void createCloth(int div, float w) {

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<unsigned int> indices;

	float triangleSide = w / div;

	for (int row = 0; row < div + 1; row++) {
		for (int col = 0; col < div + 1; col++) {
			glm::vec3 vert = glm::vec3(col * triangleSide, 0.0f, row * -triangleSide);
			vertices.push_back(vert.x);
			vertices.push_back(vert.y);
			vertices.push_back(vert.z);

			normals.push_back(0.0f);
			normals.push_back(0.0f);
			normals.push_back(1.0f);
		}
	}

	for (int row = 0; row < div; row++) {
		for (int col = 0; col < div; col++) {
			int index = row * (div + 1) + col;

			indices.push_back(index);
			indices.push_back(index + (div + 1) + 1);
			indices.push_back(index + (div + 1));

			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index +(div + 1) + 1);
		}
	}

	cloth = Mesh();
	GLfloat* temp = vertices.data();
	cloth.CreateMesh(vertices.data(), indices.data(), vertices.size(), indices.size());
}

int main()
{
	mainWindow = Window(1920, 1080, false);
	mainWindow.Initialise();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	//imgui setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.FontGlobalScale = 1.25f;
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	ImGui_ImplGlfw_InitForOpenGL(mainWindow.getGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	Shader* sRenderPass = new Shader();

	sRenderPass->CreateFromFiles(vShader, "", fShader);

	camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.05f);

	dullMaterial = Material(0.5f, 256);

	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	
	defaultTexture = Texture("Textures/fallback.png");
	defaultTexture.LoadTexture();

	debugCube = Model();
	debugCube.LoadModel("Models/cube.obj");

	debugPlane = Model();
	debugPlane.LoadModel("Models/plane.obj");

	createCloth(5,5);

	printf("Initial loading took: %f seconds\n", glfwGetTime());

	mainDirectionalLight = DirectionalLight(1.0f, 1.0f, 1.0f,
											0.5f, 0.0f,
											-1.0f, -1.0f, -1.0f, 
											1024, 1024);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 200.0f);

	double lastFrame = 0.0f;
	int frameCount = 0;
	int fps = 0;
	glfwSetTime(0.0f);

	int incr = 1;

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
			std::string newTitle = "artisEngine - " + FPS + "FPS";
			glfwSetWindowTitle(mainWindow.getGLFWWindow(), newTitle.c_str());
		}

		glfwPollEvents();
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		camera.keyControl(mainWindow.getKeys(), deltaTime);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("finally working");
		ImGui::SliderInt("increment texture", &incr,0,100);
		//ImGui::DragFloat3("lightdir", glm::value_ptr(dir), 0.01f, -100.0f, 100.0f);
		//ImGui::DragFloat4("flashlight", glm::value_ptr(temp), 0.01f, -10.0f, 10.0f);
		//ImGui::SliderFloat("left", &orthoLeft, -100.0f, 100.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		sRenderPass->UseShader();

		uniformModel = sRenderPass->GetModelLocation();
		uniformProjection = sRenderPass->GetProjectionLocation();
		uniformView = sRenderPass->GetViewLocation();
		uniformEyePosition = sRenderPass->GetEyePositionLocation();
		uniformSpecularIntensity = sRenderPass->GetSpecularIntensityLocation();
		uniformShininess = sRenderPass->GetShininessLocation();
		uniformLightSpace = sRenderPass->GetLightSpaceMatrixLocation();
		uniformSkybox = sRenderPass->GetSkyBoxLocation();
		uniformLightFarPlane = sRenderPass->GetLightFarPlane();

		glViewport(0, 0, mainWindow.getBufferWidth(), mainWindow.getBufferHeight());
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sRenderPass->SetDirectionalLight(&mainDirectionalLight);
		sRenderPass->SetTexture(0);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniformMatrix4fv(uniformLightSpace, 1, GL_FALSE, glm::value_ptr(mainDirectionalLight.GetLightSpaceMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		defaultTexture.UseTexture();

		glm::mat4 model(1.0f);

		model = glm::mat4(1.0f);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		cloth.RenderMesh();

		//debugCube.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));

		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//debugPlane.RenderModel();

		glUseProgram(0);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		mainWindow.swapBuffers();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(mainWindow.getGLFWWindow());
	glfwTerminate();

	return 0;
}