#pragma once
#include "Light.h"
#include "DShadowMap.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
					GLfloat aIntensity, GLfloat dIntensity,
					GLfloat xDir, GLfloat yDir, GLfloat zDir,
					GLuint sw, GLuint sh);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientcolorLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation);

	void WriteShadowMap(GLuint uniformLightSpaceMatrixLocation);
	void UnbindShadowMap();
	void ReadShadowMap();

	DShadowMap GetDShadowMap() { return shadowMap; };

	glm::mat4 GetLightSpaceMatrix() { return lightSpaceMatrix; };
	glm::vec3 GetDirection() { return direction; };
	void SetDirection(glm::vec3 newDir);

	~DirectionalLight();

private:
	glm::vec3 direction;
	DShadowMap shadowMap;
	glm::mat4 lightProjection;
	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix;
};

