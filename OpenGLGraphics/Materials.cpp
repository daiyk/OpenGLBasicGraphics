#include "Materials.h"

Materials::Materials()
{
	specularIntensity = 0.0f;
	shininess = 0.0f;
}
Materials::Materials(GLfloat sIntensity, GLfloat shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}
void Materials::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}