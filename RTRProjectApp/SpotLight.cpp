#include "SpotLight.h"

// inherits from !PointLight! Class and adds a direction vector edge and procEdge(proccesed edge used for smooth falloff of light)
SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue, 
	GLfloat aIntensity, GLfloat dIntensity, 
	GLfloat xPos, GLfloat yPos, GLfloat zPos, 
	GLfloat xDir, GLfloat yDir, GLfloat zDir, 
	GLfloat con, GLfloat lin, GLfloat exp, 
	GLfloat edg) : PointLight(red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp, 1024,1024)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));

	edge = edg;
	procEdge = cosf(glm::radians(edge));
}

// called from Shader.SetSpotLights(...) with the corresponding locations retrieved from shaderprogram
void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientcolorLocation, 
	GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation, 
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, 
	GLuint edgeLocation)
{
	// sets Uniforms of SpotLight in shader (GPU) to the values of the SpotLight on CPU

	// = struct SpotLight.base(PointLight).base(Light).color (vec3)
	glUniform3f(ambientcolorLocation, color.x, color.y, color.z);
	// = struct SpotLight.base(PointLight).base(Light).ambientIntensity (float)
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	// = struct SpotLight.base(PointLight).base(Light).diffuseIntensity (float)
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	// = struct SpotLight.base(PointLight).position (vec3)
	glUniform3f(positionLocation, position.x, position.y, position.z);
	// = struct SpotLight.base(PointLight).constant (float)
	glUniform1f(constantLocation, constant);
	// = struct SpotLight.base(PointLight).linear (float)
	glUniform1f(linearLocation, linear);
	// = struct SpotLight.base(PointLight).exponent (float)
	glUniform1f(exponentLocation, exponent);

	// = struct SpotLight.direction (vec3)
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	// = struct SpotLight.edge (float)
	glUniform1f(edgeLocation, procEdge);
}

// function for flashlight effect that sets the position and direction 
// of the SpotLight to camera position and direction
void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

void SpotLight::SetLightDirection(glm::vec3 dir)
{
	direction = -glm::normalize(position - dir);
}

void SpotLight::SetConLinExpEdg(glm::vec4 in)
{
	constant = in.x;
	linear = in.y;
	exponent = in.z;
	edge = in.w;
}

SpotLight::~SpotLight()
{
}
