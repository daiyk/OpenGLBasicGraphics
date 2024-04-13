#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientStrength = 0.5f;
	diffuseStrength = 0.0f;
}

Light::Light(Light& light)
{
	color = light.color;
	ambientStrength = light.ambientStrength;
	diffuseStrength = light.diffuseStrength;
}

//constructor with parameters
Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat diffuseIntensity)
{
	color = glm::vec3(red, green, blue);
	ambientStrength = aIntensity;
	this->diffuseStrength = diffuseIntensity;
}

void Light::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseStrengthLoc)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientStrength);
	glUniform1f(diffuseStrengthLoc, this->diffuseStrength);
}
