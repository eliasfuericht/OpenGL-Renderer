#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class DShadowMap
{
public:
	DShadowMap();
	DShadowMap(GLuint w, GLuint h);

	virtual void Init();
	virtual void Write();
	virtual void Read(GLenum textureUnit);
	virtual void Unbind();

	int GetShadowMap() { return shadowMap; };

private:
	GLuint shadowMapFBO;
	GLuint shadowMap;
	GLuint width, height;
};

