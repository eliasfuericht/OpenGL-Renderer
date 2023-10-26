#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

// this function is just preparing the data to be sent to the GPU (so setting up the VAO, VBO and IBO to be used in RenderModel() later)
void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

	// generate Vertex Array Object(VAO) (stores information about how vertex attribute data is laid out in VBO)
	glGenVertexArrays(1, &VAO);
	// bind VAO, meaning "make VAO the current VAO"
	glBindVertexArray(VAO);

	// generate Index Buffer Object(IBO) (stores indices of vertices for efficient rendering(throw away duplicates)
	// is used during the pipeline to fetch the corresponding vertices from the VBO
	glGenBuffers(1, &IBO);
	// bind IBO to GL_ELEMENT_ARRAY_BUFFER
	// Any calls made to the GL_ELEMENT_ARRAY_BUFFER target will affect the currently bound buffer, in this case IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// copy data from indices array to Index Buffer (IBO)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	// generate Vertex Buffer Object(VBO)
	glGenBuffers(1, &VBO);
	// bind VBO to GL_ARRAY_BUFFER
	// Any calls made to the GL_ARRAY_BUFFER target will affect the currently bound buffer, in this case VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// copy data from vertices array to Vertex Buffer on GPU (VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	// Configures the vertex attribute pointers stored in the VAO
	// Explanation what the of following line does:
	// it specifies the location (in this case 0), then sets the size of the data (in this case 3 floats/vec3)
	// then the type of data (GL_FLOAT), then if the data should be normalized (GL_FALSE),
	// then the stride (meaning after 3 values have been read, jump 8 times the size of a vertices[0] bzw. float from index 0
	// to read the next 3 values) and lastly the offset (meaning start at index 0) - which is very important, idk why its last
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	// indicates that the vertex attribute at layout (location = 0) is enabled and should be used for rendering
	glEnableVertexAttribArray(0);

	// same thing as above, just for layout (location = 1), so in this case it starts at index 2 (specified by the offset (void*)(sizeof(vertices[0]) * 3)
	// and reads 2 floats/vec2, in this case its texture coordinates(x,y), and then jumps sizeof(vertices[0]) * 8 to the next informationset
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	// exact same as above but for normals, so offset is (void*)(sizeof(vertices[0]) * 5) to start right after the texture coordinates
	// and read 3 floats/vec3 and jump to next informationset
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	// unbind VBO so that it can't be modified
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// unbind IBO so that it can't be modified
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// unbind VAO so that it can't be modified
	glBindVertexArray(0);
}

void Mesh::RenderMesh()
{
	// bind/activate VAO of this mesh
	glBindVertexArray(VAO);
	// bind/activate IBO of this mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// finally the Draw Call!
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	// unbinding again
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// cleanup function
void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}


Mesh::~Mesh()
{
	ClearMesh();
}
