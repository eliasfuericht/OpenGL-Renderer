#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetShaderID() { return shaderID; };

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientcolorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetEyePositionLocation();
	GLuint GetTextureLocation();
	GLuint GetDShadowMapLocation();
	GLuint GetOShadowMapLocation();
	GLuint GetLightSpaceMatrixLocation();
	GLuint GetSkyBoxLocation();
	GLuint* GetOShadowMatrices();
	GLuint GetLightPosLocation();
	GLuint GetLightFarPlane();


	void SetDirectionalLight(DirectionalLight * dLight);
	void SetPointLights(PointLight * pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight * sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetOmniDirectionalShadowMap(GLuint textureUnit);
	void SetSkybox(GLuint textureUnit);

	void UseShader();
	void ClearShader();

	~Shader();

private:
	int pointLightCount;
	int spotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess, uniformTexture, uniformDShadowMap, uniformOShadowMap, uniformLightSpaceMatrix,
		uniformSkyBox, uniformLightPos, uniformLightFarPlane;
	GLuint uniformOShadowMatrices[6];

	struct {
		GLuint uniformcolor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformcolor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformcolor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	struct {
		GLuint uniformShadowMap;
		GLuint uniformFarPlane;
	} uniformOmniShadowMap[MAX_SPOT_LIGHTS + MAX_POINT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType);
};

