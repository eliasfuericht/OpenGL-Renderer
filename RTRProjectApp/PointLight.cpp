#include "PointLight.h"


// inherits from Light Class and adds position, constant, linear and exponent (used for lighting calculations)
PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, 
						GLfloat aIntensity, GLfloat dIntensity, 
						GLfloat xPos, GLfloat yPos, GLfloat zPos, 
						GLfloat con, GLfloat lin, GLfloat exp) : Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

//called from Shader.SetPointLights(...) with the corresponding locations retrieved from shaderprogram
void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientcolorLocation,
	GLuint diffuseIntensityLocation, GLuint positionLocation,
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	//sets Uniforms of PointLight in shader (GPU) to the values of the PointLight on CPU

	// = struct PointLight.base(Light).color (vec3)
	glUniform3f(ambientcolorLocation, color.x, color.y, color.z);
	// = struct PointLight.base(Light).ambientIntensity (float)
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	// = struct PointLight.base(Light).diffuseIntensity (float)
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	// = struct PointLight.position (vec3)
	glUniform3f(positionLocation, position.x, position.y, position.z);
	// = struct PointLight.constant (float)
	glUniform1f(constantLocation, constant);
	// = struct PointLight.linear (float)
	glUniform1f(linearLocation, linear);
	// = struct PointLight.exponent (float)
	glUniform1f(exponentLocation, exponent);
}

PointLight::~PointLight()
{
}
