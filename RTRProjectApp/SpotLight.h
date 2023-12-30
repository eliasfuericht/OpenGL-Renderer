#pragma once
#include "PointLight.h"
class SpotLight :
	public PointLight
{
public:
	SpotLight();

	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edg);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientcolorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation);

	void SetFlash(glm::vec3 pos, glm::vec3 dir);
	void SetLightDirection(glm::vec3 dir);

	void SetConLinExpEdg(glm::vec4 in);

	glm::vec4 GetConLinExpEdg() { return glm::vec4(constant, linear, exponent, edge); };


	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat edge, procEdge;
};

