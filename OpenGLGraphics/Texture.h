#pragma once
#include "stb_image.h"
#include <GL/glew.h>

class Texture
{
	public:
		Texture();
		Texture(const char* fileLoc);
		~Texture();
		bool LoadTexture();
		void UseTexture();
		void ClearTexture();
	private:
		GLuint textureID; //wrap the texture in a texture ID
		int width, height, bitDepth;
		const char* fileLocation;
};

