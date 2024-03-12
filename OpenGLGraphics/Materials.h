#pragma once
#include <GL/glew.h>
class Materials
{
public:
	Materials();
	Materials(GLfloat Intensity, GLfloat shine);
	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
private:
	GLfloat specularIntensity;
	GLfloat shininess; //the number of specular highlights, which is the exponent of the specular term
};

