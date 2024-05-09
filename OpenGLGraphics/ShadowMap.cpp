#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
	m_fbo = 0;
	m_shadowMap = 0;
}

void ShadowMap::Init(GLuint width, GLuint height) {
	glGenFramebuffers(1, &m_fbo);

	/// ---- in the following we only create the texture, but we don't bind it to any texture unit ---- ///
	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); //default value is 0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Prevents shadow map edge artifacts, we use GL_CLAMP_TO_EDGE that will return the border color for out of bounds texture coordinates
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Error creating shadow map framebuffer\n");
	}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// <summary>
/// After call this function, render the scene so that the shadow map is created and written to the texture
/// </summary>
void ShadowMap::WriteShadowMap()
{
	//bind the framebuffer, so we can render to it
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void ShadowMap::BindShadowMapTexture(int textureLayer)
{
	//bind the shadow map, so we can use it in the shader
	glActiveTexture(GL_TEXTURE0+textureLayer); // GL_TEXTURE0, GL_TEXTURE1, etc.
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
}

void ShadowMap::FinishWriteShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
ShadowMap::~ShadowMap()
{
	if (m_fbo)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}
	if (m_shadowMap) 
	{
		glDeleteTextures(1, &m_shadowMap);
	}
}


