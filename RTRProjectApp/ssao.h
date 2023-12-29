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
	Ssao();
	Ssao(GLuint w, GLuint h);

	void configureGBuffer();
	void createSsaoFrameBuffer();
	void generateKernel();
	void generateNoise();

	unsigned int getGBuffer();
	unsigned int getSsaoFBO();
	std::vector<glm::vec3> getSsaoKernel();
	unsigned int getGPosition();
	unsigned int getGNormal();
	unsigned int getGAlbedo();
	unsigned int getNoiseTexture();
	unsigned int getColorBuffer();
	unsigned int getBlurFBO();
	unsigned int getColorBufferBlur();


private:
	GLuint width, height;

	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedo;
	unsigned int rboDepth;
	unsigned int ssaoFBO, ssaoBlurFBO;
	unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
	std::vector<glm::vec3> ssaoNoise;
	unsigned int noiseTexture;

	std::uniform_real_distribution<GLfloat> randomFloats;
	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoKernel;
};
