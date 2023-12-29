#pragma once


#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <random>

class Ssao
{
public:
	void configureGBuffer(const unsigned int width, const unsigned int height);
	void createSsaoFrameBuffer(const unsigned int width, const unsigned int height);
	void generateKernel();
	void generateNoise();

	void doGeometryPass();

	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedo;
	unsigned int rboDepth;
	unsigned int ssaoFBO, ssaoBlurFBO;
	unsigned int ssaoColorBuffer, ssaoColorBufferBlur;

	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoKernel;
};
