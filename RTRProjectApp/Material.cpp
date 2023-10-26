#include "Material.h"

//Class thats currently just used for phong-shading model (specular, shininess)
Material::Material()
{
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}

// called from main.cpp to send the values of the material you want to use to the shader
void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	// sets Uniforms of Material in shader (GPU) to the values of the Material on CPU+

	// = struct Material.specularIntensity (float)
	glUniform1f(specularIntensityLocation, specularIntensity);
	// = struct Material.shininess (float)
	glUniform1f(shininessLocation, shininess);
}

Material::~Material()
{
}
