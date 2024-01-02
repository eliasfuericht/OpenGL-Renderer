#version 460

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

out vec4 FragColor;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D dShadowMap;

uniform samplerCube oShadowMap;

uniform samplerCube skybox;

uniform Material material;

uniform vec3 eyePosition;

uniform float lightFarPlane;

float CalcOmniDirectionalShadow(PointLight pLight)
{
	// get vector between fragment position and light position
	vec3 fragToLight = FragPos - pLight.position;
	// use the light to fragment vector to sample from the depth map    
	float closestDepth = texture(oShadowMap, fragToLight).r;
	// it is currently in linear range between [0,1]. Re-transform back to original value
	closestDepth *= lightFarPlane;
	// now get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);
	// now test for shadows
	float bias = 0.0005;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;
}

float CalcDirectionalShadow(DirectionalLight light)
{
	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;

	projCoords = projCoords * 0.5 + 0.5;

	float currentDepth = projCoords.z;
	
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(light.direction);

	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(dShadowMap, 0);

	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(dShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - 0.001 > pcfDepth ? 1.0 : 0.0;;
		}
	}

	shadow /= 9.0;

	if (projCoords.z > 1.0) {
		shadow = 0.0;
	}

	return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
	vec4 ambientcolor = vec4(light.color, 1.0f) * light.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffusecolor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);
	
	vec4 specularcolor = vec4(0, 0, 0, 0);
	
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularcolor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientcolor + diffusecolor + specularcolor) * (1.0 - shadowFactor);
}

vec4 CalcDirectionalLight()
{
	float shadow = CalcDirectionalShadow(directionalLight);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadow);
}

vec4 CalcPointLight(PointLight pLight)
{
	vec3 direction = FragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	
	float shadow = CalcOmniDirectionalShadow(pLight);

	vec4 color = CalcLightByDirection(pLight.base, direction, shadow);
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance +
						pLight.constant;
	
	return (color / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);
	
	if(slFactor > sLight.edge)
	{
		vec4 color = CalcPointLight(sLight.base);
		
		return color * (1.0f - (1.0f - slFactor)*(1.0f/(1.0f - sLight.edge)));
		
	} else {
		return vec4(0, 0, 0, 0);
	}
}

vec4 CalcPointLights()
{
	vec4 totalcolor = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{		
		totalcolor += CalcPointLight(pointLights[i]);
	}
	
	return totalcolor;
}

vec4 CalcSpotLights()
{
	vec4 totalcolor = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++)
	{		
		totalcolor += CalcSpotLight(spotLights[i]);
	}
	
	return totalcolor;
}

void main()
{
	vec4 finalcolor = CalcDirectionalLight();
	finalcolor += CalcPointLights();
	finalcolor += CalcSpotLights();

	/*vec3 I = normalize(FragPos - eyePosition);
	vec3 R = reflect(I, normalize(Normal));
	FragColor = vec4(texture(skybox, R).rgb, 1.0);*/
	
	FragColor = texture(theTexture, TexCoord) * finalcolor;
}