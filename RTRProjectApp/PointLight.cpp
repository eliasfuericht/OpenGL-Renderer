#include "PointLight.h"


// inherits from Light Class and adds position, constant, linear and exponent (used for lighting calculations)
PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;

	shadowMap = OShadowMap();
	float aspect = (float)shadowMap.GetWidth() / (float)shadowMap.GetHeight();
	float near = 0.1f;
	float far = 50.0f;
	shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, 
						GLfloat aIntensity, GLfloat dIntensity, 
						GLfloat xPos, GLfloat yPos, GLfloat zPos, 
						GLfloat con, GLfloat lin, GLfloat exp,
						GLuint sw, GLuint sh) : Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;

	shadowMap = OShadowMap(sw, sh);
	shadowMap.Init();
	float aspect = (float)shadowMap.GetWidth() / (float)shadowMap.GetHeight();
	float near = 0.1f;
	float far = 50.0f;
	shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
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

	UpdateShadowTransforms();
}

void PointLight::UpdateShadowTransforms() {
	shadowTransforms.clear();
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
}

void PointLight::WriteShadowMap(GLuint* uniformOShadowMatricesLocation, GLuint uniformLightPosLocation, GLuint uniformFarPlaneLocation)
{
	shadowMap.Write();
	for (int i = 0; i < 6; ++i) {
		glUniformMatrix4fv(uniformOShadowMatricesLocation[i], 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
	}
	glUniform3f(uniformLightPosLocation, position.x, position.y, position.z);
	glUniform1f(uniformFarPlaneLocation, farPlane);
}

void PointLight::UnbindShadowMap()
{
	shadowMap.Unbind();
}

void PointLight::ReadShadowMap()
{
	shadowMap.Read(GL_TEXTURE2);
}

void PointLight::SetConLinExp(glm::vec3 in)
{
	constant = in.x;
	linear = in.y;
	exponent = in.z;

}

PointLight::~PointLight()
{
}
