#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLfloat x, GLfloat y, GLfloat z, glm::vec3 direction, GLfloat c, GLfloat l, GLfloat q, GLfloat edge) : 
	PointLight(red, green, blue, aIntensity, dIntensity, x, y, z, c, l, q)
{
	this->direction = glm::normalize(direction);
	this->edge = edge;
	procEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseStrengthLoc, GLuint LightPositionLocation, GLuint LightDirectionLocation, GLuint constantLoc, GLuint linearLoc, GLuint quadraticLoc, GLuint edgeLoc)
{
	PointLight::UseLight(ambientIntensityLocation, ambientColorLocation, diffuseStrengthLoc, LightPositionLocation, constantLoc, linearLoc, quadraticLoc);
	glUniform3f(LightDirectionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLoc, procEdge);
}