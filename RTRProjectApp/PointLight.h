#pragma once

#include <vector>

#include "Light.h"
#include "OShadowMap.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLuint sw, GLuint sh);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientcolorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	void SetLightPosition(glm::vec3 pos) { position = pos; }

	glm::vec3 GetLightPosition() { return position; }

	void WriteShadowMap(GLuint* uniformOShadowMatricesLocation, GLuint uniformLightPosLocation, GLuint uniformFarPlaneLocation);
	void UnbindShadowMap();
	void ReadShadowMap();

	OShadowMap GetDShadowMap() { return shadowMap; };

	~PointLight();

protected:
	glm::vec3 position;
	float farPlane = 25.0f;

	GLfloat constant, linear, exponent;

	OShadowMap shadowMap;
	glm::mat4 shadowProj;
	std::vector<glm::mat4> shadowTransforms;
};

