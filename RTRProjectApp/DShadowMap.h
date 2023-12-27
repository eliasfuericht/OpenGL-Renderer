#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class DShadowMap
{
public:
	DShadowMap();
	DShadowMap(GLuint w, GLuint h);
	void Init();
	void Write();
	void Read();
	int GetShadowMap() { return shadowMap; };
	void Unbind();

private:
	unsigned int shadowMapFBO;
	unsigned int shadowMap;
	GLuint width, height;
};

