#pragma once
#include "PointLight.h"
class SpotLight: public PointLight
{
	public:
		SpotLight();
		SpotLight(GLfloat red, GLfloat green, GLfloat blue, 
			GLfloat aIntensity, GLfloat dIntensity, 
			GLfloat x, GLfloat y, GLfloat z, 
			glm::vec3 direction, 
			GLfloat c, GLfloat l, GLfloat q, 
			GLfloat edge, GLfloat outerEdge,
			GLfloat farPlane = 0.0f);
		void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseStrengthLoc,
			GLuint LightPositionLocation, GLuint LightDirectionLocation, 
			GLuint constantLoc, GLuint linearLoc, GLuint quadraticLoc, 
			GLuint edgeLoc, GLuint outerEdgeLoc);
		void SetPose(glm::vec3 pos, glm::vec3 dir);
	private:
		glm::vec3 direction;
		GLfloat edge, softOuterEdge, procEdge, procOuterEdge;//cosine of the edge radian
};

