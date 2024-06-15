#pragma once
#include <GL/glew.h>
#include <stdio.h>
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();
	virtual bool Init(GLuint width, GLuint height);
	virtual void WriteShadowMap();
	virtual int BindShadowMapTexture(int textureLayer); // Bind the shadow map to a texture layer, GL_TEXTURE0, GL_TEXTURE1, etc. and return the latest textureUnit index
	virtual void FinishWriteShadowMap();
protected:
	GLuint m_fbo = 0; // Frame buffer object for the depth buffer
	GLuint m_shadowMap = 0; // Texture to store the depth buffer
	GLfloat m_aspect;
};

