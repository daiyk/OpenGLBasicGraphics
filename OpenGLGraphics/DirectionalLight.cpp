#include "DirectionalLight.h"

void DirectionalLight::CreateShadowMap(GLuint width, GLuint height, GLfloat nearPlane, GLfloat farPlane)
{
	__super::CreateShadowMap(width, height, nearPlane, farPlane);
	//create the orthographic projection matrix
	lightTransform = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, nearPlane, farPlane) * glm::lookAt(-direction, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}