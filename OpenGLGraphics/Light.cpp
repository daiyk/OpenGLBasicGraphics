#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientStrength = 0.5f;
	diffuseStrength = 0.0f;
	shadowMapWidth = 1024;
	shadowMapHeight = 1024;
	nearPlane = 0.1f;
	farPlane = 100.0f;
}

Light::Light(Light& light)
{
	color = light.color;
	ambientStrength = light.ambientStrength;
	diffuseStrength = light.diffuseStrength;
	shadowMapWidth = light.shadowMapWidth;
	shadowMapHeight = light.shadowMapHeight;
	nearPlane = light.nearPlane;
	farPlane = light.farPlane;
}

void Light::CreateShadowMap(GLuint width, GLuint height,GLfloat nearPlane, GLfloat farPlane)
{
	shadowMapWidth = width;
	shadowMapHeight = height;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	shadowMap = std::make_unique<ShadowMap>();
	shadowMap->Init(shadowMapWidth, shadowMapHeight);
}

//constructor with parameters
Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat diffuseIntensity)
{
	color = glm::vec3(red, green, blue);
	ambientStrength = aIntensity;
	this->diffuseStrength = diffuseIntensity;
	shadowMapWidth = 1024;
	shadowMapHeight = 1024;
	nearPlane = 0.1f;
	farPlane = 100.0f;
}

void Light::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseStrengthLoc)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientStrength);
	glUniform1f(diffuseStrengthLoc, this->diffuseStrength);
}
