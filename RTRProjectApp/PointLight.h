#pragma once
#include "Light.h"

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientcolorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	void SetLightPosition(glm::vec3 pos) { position = pos; }

	glm::vec3 GetLightPosition() { return position; }

	~PointLight();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;
};

