#version 460
out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube skybox;
uniform samplerCube oShadowMap;

void main()
{
    FragColor = texture(skybox, TexCoord);
}