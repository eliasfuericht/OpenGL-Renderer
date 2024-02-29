#pragma once

#include <iostream>
#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void UpdateMesh(GLfloat* vertices);
	void ClearMesh();

	~Mesh();

public:
	unsigned int vertexCount;
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;

	
};

