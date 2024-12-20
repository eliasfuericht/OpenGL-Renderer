#include "Light.h"

// Base class for all Lights
// Does nothing other than initialize the variables
Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}

void Light::SetAmbDif(GLfloat a, GLfloat d)
{
	ambientIntensity = a;
	diffuseIntensity = d;
}

Light::~Light()
{
}
