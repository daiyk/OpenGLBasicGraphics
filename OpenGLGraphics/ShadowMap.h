#pragma once
#include <GL/glew.h>
#include <stdio.h>
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();
	void Init(GLuint width, GLuint height);
	virtual void WriteShadowMap();
	virtual void BindShadowMapTexture(int textureLayer); // Bind the shadow map to a texture layer, GL_TEXTURE0, GL_TEXTURE1, etc.
	virtual void FinishWriteShadowMap();
protected:
	GLuint m_fbo = 0; // Frame buffer object for the depth buffer
	GLuint m_shadowMap = 0; // Texture to store the depth buffer
};

