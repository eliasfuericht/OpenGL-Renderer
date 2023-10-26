#include "Model.h"

Model::Model()
{
}


void Model::RenderModel()
{
	// iterates over all meshes in the meshList vector
	for (size_t i = 0; i < meshList.size(); i++) 
	{	
		// gets the index of the material used by the current mesh
		unsigned int materialIndex = meshToTex[i];
	
		// checks if the materialIndex is within the bounds of the textureList vector and if the texture exists at this index
		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			// if so, use the texture at this index for this mesh
			textureList[materialIndex]->UseTexture();
		}
		// renders the current mesh
		meshList[i]->RenderMesh();
	}
}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	// loads the entire file into an aiScene object
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene) {
		printf("Model %s failed to load: %s", fileName, importer.GetErrorString());
		printf("model failed to load");
		return;
	}
	// passes the root node of the scene along with the scene itself 
	// for data extraction (vertices, texturecoordinates, normals, indices)
	LoadNode(scene->mRootNode, scene);

	// passes the scene to LoadMaterials() for data extraction
	LoadMaterials(scene);
}


void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	// iterates over all meshes in the scene and calls LoadMesh() for each one
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	// recursively calls LoadNode() for each child of the current node
	for (size_t i = 0; i < node->mNumChildren; i++) {
		LoadNode(node->mChildren[i], scene);
	}
}

// Here all the data gets extracted from each mesh
void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	// vectors to store vertices and indices
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	// iterates over all vertices of the mesh
	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		// stores position (x,y,z) in the first three indices of the vertices vector
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		// if the mesh has texture coordinates, stores them in the next two indices of the vertices vector
		if (mesh->mTextureCoords[0]) {
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		// otherwise, stores 0.0f, 0.0f instead (so that the shader doesn't break)
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		// lastly, stores the normal vector (x,y,z) in the last three indices of the vertices vector
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
		// so in total per vertex, we have 8 floats (3 for position, 2 for texture coordinates, 3 for normals)
		// ==> this is why the stride is sizeof(vertices[0]) * 8 in the vertex attribute pointer in Mesh.cpp (glVertexAttribPointer())
	}

	// iterates over all faces of the mesh
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		// extracts current face from mesh
		aiFace face = mesh->mFaces[i];
		// iterates over all indices of the current face
		for (size_t j = 0; j < face.mNumIndices; j++) {
			// stores the indices of the current face in the indices vector
			indices.push_back(face.mIndices[j]);
		}
	}

	// create new mesh object and pass the vertices and indices vectors we just extracted
	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());

	// push the new mesh into the meshList vector
	meshList.push_back(newMesh);

	// Apparently the index of the used Material (i dont really understand this...)
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
	// resizes vector to mNumMaterials
	textureList.resize(scene->mNumMaterials);

	// iterates over all materials in the scene
	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		// stores current material in material
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		// checks if the current material has a texture (specifically checks for diffuse texture)
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			// checks if the texture can be retrieved from the material and stores address in path
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				// finds the last occurence of a backslash in the path string to isolate the filename
				int idx = std::string(path.data).rfind("\\");
				// creates a substring of the path string + 1 to extract the filename
				std::string filename = std::string(path.data).substr(idx + 1);

				// creates new path string to the texture to be used from the Textures folder
				std::string texPath = std::string("Textures/") + filename;

				// creates new texture object
				textureList[i] = new Texture(texPath.c_str());

				// loades the texture and checks if it was successful
				if (!textureList[i]->LoadTexture()) {
					printf("Failed to load texture at: %s", texPath);
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		// if the current material has no texture, a fallback texture is used
		if (!textureList[i])
		{
			textureList[i] = new Texture("Textures/fallback.png");
			textureList[i]->LoadTexture();
		}
	}
}

// cleanup function
void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++) {
		if (meshList[i]) {
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++) {
		if (textureList[i]) {
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

Model::~Model()
{
}
