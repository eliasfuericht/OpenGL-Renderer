#version 460

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

out vec4 FragPos;

uniform mat4 shadowMatrices[6];

void main()
{
    for (gl_Layer = 0; gl_Layer < 6; ++gl_Layer)
    {
        for (int i = 0; i < 3; ++i) // for each triangle vertex
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[gl_Layer] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}