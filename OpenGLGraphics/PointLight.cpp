#include "PointLight.h"
#include "OmniShadowMap.h"
#include <glm/ext/matrix_clip_space.hpp>

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLfloat x, GLfloat y, GLfloat z, GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat farPlane) : Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(x, y, z);
	this->constant = constant;
	this->linear = linear;
	this->exponent = exponent;
	this->farPlane = farPlane;
}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseStrengthLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	__super::UseLight(ambientIntensityLocation, ambientColorLocation, diffuseStrengthLocation);
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

bool PointLight::CreateShadowMap(GLuint width, GLuint height, GLfloat nearPlane, GLfloat farPlane)
{
	this->farPlane = farPlane;
	shadowMap = std::make_unique<OmniShadowMap>();
	//use pointer to initialize the unique_ptr obj
	if (!shadowMap->Init(nearPlane, farPlane)) {
		return false;
	}
	//compute projection matrix
	projMatrix = glm::perspective(glm::radians(90.0f), (GLfloat)width / height, nearPlane, farPlane);
	//compute six light transforms for each face
	lightTransforms.push_back(projMatrix * glm::lookAt(position, position + glm::vec3(1, 0, 0), glm::vec3(0.0, 1.0, 0.0)));
	lightTransforms.push_back(projMatrix * glm::lookAt(position, position + glm::vec3(-1, 0, 0), glm::vec3(0.0, 1.0, 0.0)));
	lightTransforms.push_back(projMatrix * glm::lookAt(position, position + glm::vec3(0, 1, 0), glm::vec3(0.0, 1.0, 0.0)));
	lightTransforms.push_back(projMatrix * glm::lookAt(position, position + glm::vec3(0, -1, 0), glm::vec3(0.0, 1.0, 0.0)));
	lightTransforms.push_back(projMatrix * glm::lookAt(position, position + glm::vec3(0, 0, 1), glm::vec3(0.0, 1.0, 0.0)));
	lightTransforms.push_back(projMatrix * glm::lookAt(position, position + glm::vec3(0, 0, -1), glm::vec3(0.0, 1.0, 0.0)));
	return true;
}
