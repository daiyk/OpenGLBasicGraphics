#pragma once
#include <GL/glew.h>
#include <assimp/material.h>
class Materials
{
public:
	Materials();
	Materials(aiMaterial* material); // constructor for assimp package material
	Materials(GLfloat Intensity, GLfloat shine);
	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
private:
	GLfloat specularIntensity;
	GLfloat shininess; //the number of specular highlights, which is the exponent of the specular term
	aiMaterial* _material = nullptr;
};

