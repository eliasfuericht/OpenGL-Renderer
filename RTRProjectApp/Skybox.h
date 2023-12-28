#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Texture.h"

class Skybox
{
public:
	Skybox();
	Skybox(std::string folder);
	unsigned int LoadCubemap();
	void RenderSkybox(GLuint projectionLocation, glm::mat4 projection, GLuint viewLocation, glm::mat4 view, GLuint modelLocation);

private:
	std::vector<std::string> faces {};
	GLuint cubemapTexture;
};

