#include "DirectionalLight.h"

// inherits from Light Class and adds a direction vector
DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
									GLfloat aIntensity, GLfloat dIntensity,
									GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

// called from Shader.SetDirectionalLight(...) with the corresponding locations retrieved from shaderprogram
void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientcolorLocation,
	GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	// sets Uniforms of DirectionalLight in shader (GPU) to the values of the DirectionalLight on CPU
	// if you compare these lines to the lines in fragment.glsl you will see that the structure is the same

	// = struct DirectionalLight.base(Light).color (vec3)
	glUniform3f(ambientcolorLocation, color.x, color.y, color.z);
	// = struct DirectionalLight.base(Light).ambientIntensity (float)
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	// = struct DirectionalLight.base(Light).diffuseIntensity (float)
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	// = struct DirectionalLight.direction (vec3)
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

DirectionalLight::~DirectionalLight()
{
}
