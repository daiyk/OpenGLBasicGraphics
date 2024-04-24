#pragma once
#include "stb_image.h"
#include <GL/glew.h>
#include <assimp/material.h>

class Texture
{
	public:
		Texture();
		Texture(const char* fileLoc);
		//constructor for assimp package material's texture
		Texture(const char* fileLoc, aiTextureType type);
		~Texture();
		void ResetTexture(const char* fileLoc);
		bool LoadTexture();
		bool LoadTextureWithAlpha();
		void UseTexture(GLuint layer = 0);
		void ClearTexture();
	    //get assimpTextureType
		aiTextureType GetTextureType() { return assimpTextureType; }
	private:
		GLuint textureID; //wrap the texture in a texture ID
		aiTextureType assimpTextureType = aiTextureType::aiTextureType_NONE;
		int width, height, bitDepth;
		const char* fileLocation;
};

