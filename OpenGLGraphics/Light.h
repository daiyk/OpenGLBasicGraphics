#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <memory>
#include "ShadowMap.h"
//define a constant of number of point light
#define NUM_POINT_LIGHTS 5
#define NUM_SPOT_LIGHTS 5

class Light
{
	public:
		Light(); 
		Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat diffuseIntensity);
		Light(Light& light);
		virtual void CreateShadowMap(GLuint width, GLuint height, GLfloat nearPlane, GLfloat farPlane);
		void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseStrength);
		GLuint ShadowMapWidth() { return shadowMapWidth; }
		GLuint ShadowMapHeight() { return shadowMapHeight; }
	protected:
		glm::vec3 color;
		GLfloat ambientStrength;
		GLfloat diffuseStrength;
		//shadow map components
		std::unique_ptr<ShadowMap> shadowMap;
		GLuint shadowMapWidth, shadowMapHeight;
		GLfloat nearPlane, farPlane;
};

