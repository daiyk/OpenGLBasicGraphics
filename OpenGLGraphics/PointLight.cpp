#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLfloat x, GLfloat y, GLfloat z, GLfloat constant, GLfloat linear, GLfloat exponent) : Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(x, y, z);
	this->constant = constant;
	this->linear = linear;
	this->exponent = exponent;

}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseStrengthLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	__super::UseLight(ambientIntensityLocation, ambientColorLocation, diffuseStrengthLocation);
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}