#include "OShadowMap.h"

OShadowMap::OShadowMap()
{
    cubeShadowMap = 0;
    cubeShadowMapFBO = 0;
    width = 1024;
    height = 1024;
}

OShadowMap::OShadowMap(GLuint w, GLuint h)
{
    cubeShadowMap = 0;
    cubeShadowMapFBO = 0;
    width = w;
    height = h;
}

void OShadowMap::Init()
{
    glGenFramebuffers(1, &cubeShadowMapFBO);
    // create depth cubemap texture
    glGenTextures(1, &cubeShadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeShadowMap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, cubeShadowMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeShadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("Error creating OShadowMapFBO: %i\n", status);
    }
}

void OShadowMap::Write()
{
    glBindFramebuffer(GL_FRAMEBUFFER, cubeShadowMapFBO);

    glViewport(0, 0, width, height);

    glClear(GL_DEPTH_BUFFER_BIT);
}

void OShadowMap::Read(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeShadowMap);
}

void OShadowMap::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
