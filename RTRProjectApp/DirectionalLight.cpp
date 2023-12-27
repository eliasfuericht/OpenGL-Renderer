#include "DirectionalLight.h"

// inherits from Light Class and adds a direction vector
DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	shadowMap = DShadowMap();
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
									GLfloat aIntensity, GLfloat dIntensity,
									GLfloat xDir, GLfloat yDir, GLfloat zDir,
									GLuint sw, GLuint sh) : Light(red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
	shadowMap = DShadowMap(sw, sh);
	shadowMap.Init();
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

void DirectionalLight::WriteShadowMap(GLuint uniformLightSpaceMatrixLocation)
{
	shadowMap.Write();
	lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, -30.0f, 50.0f);
	lightView = glm::lookAt(direction, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	glUniformMatrix4fv(uniformLightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
}

void DirectionalLight::UnbindShadowMap()
{
	shadowMap.Unbind();
}

void DirectionalLight::ReadShadowMap()
{
	shadowMap.Read();
}

void DirectionalLight::SetDirection(glm::vec3 newDir) {
	direction = newDir;
}

DirectionalLight::~DirectionalLight()
{
}
