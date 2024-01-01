#pragma once
#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class OShadowMap {
public:
    OShadowMap();
    OShadowMap(GLuint w, GLuint h);

    void Init();
    void Write();
    void Read(GLenum textureUnit);
    void Unbind();

    GLuint GetWidth() { return width; };
    GLuint GetHeight() { return height; };

private:
    GLuint cubeShadowMapFBO;
    GLuint cubeShadowMap;
    GLuint width, height;
};

