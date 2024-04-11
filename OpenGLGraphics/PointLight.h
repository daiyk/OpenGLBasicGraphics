#pragma once
#define NUM_POINT_LIGHTS 5
#include "Light.h"
class PointLight :
    public Light
{
public :
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLfloat x, GLfloat y, GLfloat z, GLfloat constant, GLfloat linear, GLfloat exponent);
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);
protected:
	glm::vec3 position;
	GLfloat constant, linear, exponent;
};

