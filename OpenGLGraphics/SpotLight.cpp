#include "SpotLight.h"
SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
	edge = 0.0f;
	softOuterEdge = 0.0f;
	procEdge = cosf(glm::radians(edge));
	procOuterEdge = cosf(glm::radians(softOuterEdge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLfloat x, GLfloat y, GLfloat z, glm::vec3 direction, GLfloat c, GLfloat l, GLfloat q, GLfloat edge, GLfloat outerEdge, GLfloat farPlane) :
	PointLight(red, green, blue, aIntensity, dIntensity, x, y, z, c, l, q, farPlane)
{
	this->direction = glm::normalize(direction);
	this->edge = edge;
	this->softOuterEdge = outerEdge;
	this->procEdge = cosf(glm::radians(edge));
	this->procOuterEdge = cosf(glm::radians(outerEdge));
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseStrengthLoc, GLuint LightPositionLocation, GLuint constantLoc, GLuint linearLoc, GLuint quadraticLoc, GLuint LightDirectionLocation, GLuint edgeLoc, GLuint outerEdgeLoc)
{
	__super::UseLight(ambientIntensityLocation, ambientColorLocation, diffuseStrengthLoc, LightPositionLocation, constantLoc, linearLoc, quadraticLoc);
	glUniform3f(LightDirectionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLoc, procEdge);
	glUniform1f(outerEdgeLoc, procOuterEdge);
}

void SpotLight::SetPose(glm::vec3 pos, glm::vec3 dir)
{
	//reset the direction
	this->direction = glm::normalize(dir);
	this->position = pos;
}