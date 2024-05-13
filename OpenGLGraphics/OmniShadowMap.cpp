#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap() : ShadowMap()
{

}

bool OmniShadowMap::Init(GLuint width, GLuint height)
{
	m_width = width;
	m_height = height;

	m_aspect = (GLfloat)width / (GLfloat)height;

	glGenFramebuffers(1, &m_fbo);
	glGenTextures(1, &m_shadowMap);
	//generate six textures for each face of the cube map
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
	for (size_t i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
	//it is also ok to use_GL_TEXTURE_2D
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{ 
		printf("Framebuffer Error: %i\n", status);
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//compute the projection matrix
	return true;
}

/// <summary>
/// write the shadow map to the frame buffer
/// </summary>
void OmniShadowMap::WriteShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

/// <summary>
/// bind the shadow map to the texture unit
/// </summary>
/// <param name="textureUnit"></param>
void OmniShadowMap::BindShadowMapTexture(int textureLayer)
{
	glActiveTexture(GL_TEXTURE0+textureLayer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
}