#include "DShadowMap.h"

DShadowMap::DShadowMap()
{
	shadowMap = 0;
	shadowMapFBO = 0;
	width = 1024;
	height = 1024;
}

DShadowMap::DShadowMap(GLuint w, GLuint h)
{
	shadowMap = 0;
	shadowMapFBO = 0;
	width = w;
	height = h;
}


void DShadowMap::Init()
{
	//setting up shadowMap-Texture
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &shadowMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error creating DShadowMapFBO: %i\n", status);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);

	glViewport(0, 0, width, height);

	glClear(GL_DEPTH_BUFFER_BIT);
}

void DShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}

void DShadowMap::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
