#pragma once
#include "ShadowMap.h"
#include <glm/glm.hpp>
class OmniShadowMap :
    public ShadowMap
{
public:
    OmniShadowMap();
    bool Init(GLuint width, GLuint height) override;
    void WriteShadowMap() override;
    int BindShadowMapTexture(int textureLayer) override;
private:
    GLuint m_width, m_height;
};

