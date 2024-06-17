#pragma once
#define NUM_POINT_LIGHTS 5
#include "Light.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
class PointLight :
    public Light
{
public :
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, 
		GLfloat x, GLfloat y, GLfloat z, GLfloat constant, GLfloat linear, GLfloat exponent, 
		GLfloat farPlane = 0.0f);
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseLocation, 
		GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);
	bool CreateShadowMap(GLuint width, GLuint height, GLfloat nearPlane, GLfloat farPlane) override;
	GLfloat GetFarPlane() { return farPlane; }
	ShadowMap* GetShadowMap() { return shadowMap.get(); }
	glm::vec3 GetPosition() { return position; }
	std::vector<glm::mat4>* GetLightTransforms() { return &lightTransforms; }
protected:
	glm::vec3 position;
	GLfloat constant, linear, exponent;
	GLfloat farPlane; //how far the light will go
	std::vector<glm::mat4> lightTransforms; //six light transforms for faces
	glm::mat4 projMatrix;
};

