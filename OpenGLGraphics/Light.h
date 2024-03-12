#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
//define a constant of number of point light
#define NUM_POINT_LIGHTS 4

class Light
{
	public:
		Light(); 
		Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat x, GLfloat y, GLfloat z, glm::vec3 direction, GLfloat diffuseIntensity);

		void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseLocation, GLuint diffuseStrength);
protected:
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 direction; //if the light is a directional light
	GLfloat ambientStrength;
	GLfloat diffuseStrength;
};

