#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	CompileShader(vertexCode, geometryCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	// read vertex and fragment shaders from files
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	//checks if pipline is initialized with geometry shader
	if (strlen(geometryLocation) != 0) {
		std::string geometryString = ReadFile(geometryLocation);
		const char* geometryCode = geometryString.c_str();
		CompileShader(vertexCode, geometryCode, fragmentCode);
	}
	else {
		CompileShader(vertexCode, "", fragmentCode);
	}

}

// function to parse shader file and return its content as a string
std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	// creates shaderprogram and returns its ID - a shaderprogram is a collection of shaders (not just vertex or just fragment, both combined!)
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shaderprogram!\n");
		return;
	}

	// compile vertex and fragment shaders and attach them to shaderprogram
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);
	if (strlen(geometryCode) != 0) {
		AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	}

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// links shaderprogram to OpenGL pipeline (if vertex/fragment shaders are found -> executable is created)
	glLinkProgram(shaderID);
	// check if linking worked and store evaluation in result
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		// if linking failed, store error message in eLog
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	// checks if executable (created by glLinkProgram) can be executed given current OpenGL state.
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	// stores location of uniform variables of shaderprogram in corresponding membervariables
	//vertex shader
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformLightSpaceMatrix = glGetUniformLocation(shaderID, "lightSpaceMatrix");

	//fragment shader
	uniformDirectionalLight.uniformcolor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	// also stores location of uniform variables of shaderprogram in corresponding membervariables
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };
		// stores char* into "locBuff" for retrieving uniform location of pointLights[i].base.color
		// has to be done this way because glGetUniformLocation() only accepts char* as input
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		uniformPointLight[i].uniformcolor = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");
	
	// same for spotlights
	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
		uniformSpotLight[i].uniformcolor = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}

	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDShadowMap = glGetUniformLocation(shaderID, "dShadowMap");
	uniformOShadowMap = glGetUniformLocation(shaderID, "oShadowMap");
	uniformSkyBox = glGetUniformLocation(shaderID, "skybox");

	for (int i = 0; i < 6; ++i) {
		std::string matrixName = "shadowMatrices[" + std::to_string(i) + "]";
		uniformOShadowMatrices[i] = glGetUniformLocation(shaderID, matrixName.c_str());
	}

	uniformLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformlightFarPlane = glGetUniformLocation(shaderID, "lightFarPlane");
}

// function to compile shaderCode and attach to shaderprogram
void Shader::AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType)
{
	// creates shader object of specified type and returns its ID
	GLuint theShader = glCreateShader(shaderType);

	// transforms shader code into a format that OpenGL can understand
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	// specify length of shader code
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	// sets the source code in shader object (accessed by ID) to the source code in shaderCode (=our defined shader)
	glShaderSource(theShader, 1, theCode, codeLength);

	// compiles shader object/code
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// retrieves compile status of shader object and stores it in result
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	// attaches shader object to shaderprogram (created in CompileShader())
	glAttachShader(shaderProgram, theShader);
}


void Shader::SetDirectionalLight(DirectionalLight * dLight)
{
	// not iterating over anything because there will be only 1 directionalLight in the scene
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformcolor,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight * pLight, unsigned int lightCount)
{
	// MAX_POINT_LIGHTS is defined in CommonValues.h
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	// sets uniform variable of pointLightCount in fragment.glsl
	// = uniform pointLightCount (int);
	glUniform1i(uniformPointLightCount, lightCount);

	// iterates over all pointLights in scene and calls UseLight()
	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformcolor,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}
}

void Shader::SetSpotLights(SpotLight * sLight, unsigned int lightCount)
{
	// MAX_SPOT_LIGHTS is defined in CommonValues.h
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	// sets uniform variable of spotLightCount in fragment.glsl
	// = uniform spotLightCount (int);
	glUniform1i(uniformSpotLightCount, lightCount);

	// iterates over all spotLights in scene and calls UseLight()
	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformcolor,
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge);
	}
}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDShadowMap, textureUnit);
}

void Shader::SetOmniDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformOShadowMap, textureUnit);
}


void Shader::SetSkybox(GLuint textureUnit)
{
	glUniform1i(uniformSkyBox, textureUnit);
}

void Shader::UseShader()
{
	// sets executables of shaderprogram (at location ShaderID) to be used for rendering
	glUseProgram(shaderID);
}

// Getters for the uniform variable locations (used in main.cpp to retrieve uniform variable locations)
GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}
GLuint Shader::GetModelLocation()
{
	return uniformModel;
}
GLuint Shader::GetViewLocation()
{
	return uniformView;
}
GLuint Shader::GetAmbientcolorLocation()
{
	return uniformDirectionalLight.uniformcolor;
}
GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}
GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDirectionalLight.uniformDiffuseIntensity;
}
GLuint Shader::GetDirectionLocation()
{
	return uniformDirectionalLight.uniformDirection;
}
GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}
GLuint Shader::GetShininessLocation()
{
	return uniformShininess;
}
GLuint Shader::GetEyePositionLocation()
{
	return uniformEyePosition;
}

GLuint Shader::GetTextureLocation()
{
	return uniformTexture;
}

GLuint Shader::GetDShadowMapLocation()
{
	return uniformDShadowMap;
}

GLuint Shader::GetOShadowMapLocation()
{
	return uniformOShadowMap;
}

GLuint Shader::GetLightSpaceMatrixLocation()
{
	return uniformLightSpaceMatrix;
}

GLuint Shader::GetSkyBoxLocation() 
{
	return uniformSkyBox;
}

GLuint* Shader::GetOShadowMatrices()
{
	return uniformOShadowMatrices;
}

GLuint Shader::GetLightPosLocation()
{
	return uniformLightPos;
}

GLuint Shader::GetLightFarPlane()
{
	return uniformlightFarPlane;
}

// cleanup function
void Shader::ClearShader()
{
	std::cout << "deleted shader" << std::endl;
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
	ClearShader();
}
