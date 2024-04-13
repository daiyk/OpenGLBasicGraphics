#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
//define a constant of number of point light
#define NUM_POINT_LIGHTS 5
#define NUM_SPOT_LIGHTS 5

class Light
{
	public:
		Light(); 
		Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat diffuseIntensity);
		Light(Light& light);
		void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseStrength);
protected:
	glm::vec3 color;
	GLfloat ambientStrength;
	GLfloat diffuseStrength;
};

