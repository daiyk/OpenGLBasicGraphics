#include "Light.h"

Light::Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientStrength = 0.5f;
	diffuseStrength = 0.0f;
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
}

//constructor with parameters
Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat x, GLfloat y, GLfloat z, glm::vec3 direction, GLfloat diffuseIntensity)
{
	position = glm::vec3(x, y, z);
	color = glm::vec3(red, green, blue);
	ambientStrength = aIntensity;
	this->direction = -glm::vec3(direction);
	this->diffuseStrength = diffuseIntensity;
}

void Light::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint LightDirectionLocation, GLuint diffuseStrengthLoc)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientStrength);
	if (LightDirectionLocation != 0)
	{
		glUniform3f(LightDirectionLocation, direction.x, direction.y, direction.z);
	}
	glUniform1f(diffuseStrengthLoc, this->diffuseStrength);
}
