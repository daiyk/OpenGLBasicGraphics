#pragma once
#include "Texture.h"
#include <GL/glew.h>
#include <assimp/material.h>
#include <vector>
class Materials
{
public:
	Materials();
	Materials(aiMaterial* material); // constructor for assimp package material
	Materials(GLfloat Intensity, GLfloat shine);
	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
	//public variables
	std::vector<size_t> textureIndices;
private:
	GLfloat specularIntensity;
	GLfloat shininess; //the number of specular highlights, which is the exponent of the specular term
	aiMaterial* _material = nullptr;
	//stores the indices to the textures in the texture vector
	
};

