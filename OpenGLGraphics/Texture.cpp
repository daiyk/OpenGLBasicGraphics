#include "Texture.h"
#include <filesystem>

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
}

Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

//function that construct with 
Texture::Texture(const char* fileLoc, aiTextureType type)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
	assimpTextureType = type;
}

Texture::~Texture()
{
	ClearTexture();
}

/// <summary>
/// load texture from file with only RGB channels
/// </summary>
/// <returns></returns>
bool Texture::LoadTexture() {
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0); //load the image
	if (!texData) {
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}
	glGenTextures(1, &textureID); //generate the texture
	glBindTexture(GL_TEXTURE_2D, textureID); //bind the texture
	//set the texture parameters
	//NOTE: in opengl vec4.xyzw == vec4.rgba == vec4.strq, S and T represent the x and y coordinates of the texture or u and v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //repeat the texture if it is too small
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //repeat the texture if it is too small
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //if the texture is too big, linearly interpolate it
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //if the texture is too small, linearly interpolate it
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData); //create the texture
	glGenerateMipmap(GL_TEXTURE_2D); //generate the texture
	glBindTexture(GL_TEXTURE_2D, 0); //unbind the texture
	stbi_image_free(texData); //free the image

	return true;
}

//load texture from file if it has alpha channel
bool Texture::LoadTextureWithAlpha() {
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0); //load the image
	if (!texData) {
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}
	glGenTextures(1, &textureID); //generate the texture
	glBindTexture(GL_TEXTURE_2D, textureID); //bind the texture
	//set the texture parameters
	//NOTE: in opengl vec4.xyzw == vec4.rgba == vec4.strq, S and T represent the x and y coordinates of the texture or u and v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //repeat the texture if it is too small
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //repeat the texture if it is too small
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //if the texture is too big, linearly interpolate it
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //if the texture is too small, linearly interpolate it
	//Two GL_RGBA, first indicates the original format, the second indicates the our expected loading format (result) of the image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); //create the texture WITH ALPHA channel
	glGenerateMipmap(GL_TEXTURE_2D); //generate the texture
	glBindTexture(GL_TEXTURE_2D, 0); //unbind the texture
	stbi_image_free(texData); //free the image

	return true;
}

void Texture::UseTexture(GLuint layer)
{
	//set the texture to the texture unit number 0
	//deactivate any previous loaded texture and then active and bind this texture
	
	glActiveTexture(GL_TEXTURE0+layer); //activate the texture in the specific location
	glBindTexture(GL_TEXTURE_2D, textureID); //bind the texture
	//in fragment shader, use uniform sampler2D texture0 to access the texture
}

void Texture::ResetTexture(const char* fileLoc)
{
	ClearTexture();
	fileLocation = fileLoc;
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
}
