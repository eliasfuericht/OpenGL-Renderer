#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

bool Texture::LoadTexture()
{
	// loads texture data from file into char* array
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	// generates texture name and binds it to GL_TEXTURE_2D
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// whenever GL_TEXTURE_2D is passed - the current texture thats bound to it is being worked on (in this case flags are applied to it)
	// no texutre/image is being created/passed yet
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// now the texture(image) is being created and the data is being passed to it (via texData)
	// GL_RGB is the format of the image (in this case its RGB, no Alpha)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);

	// generates mipmaps for the texture (downscaled versions of the texture 
	// for when the texture is further away from the camera (prevents aliasing))
	glGenerateMipmap(GL_TEXTURE_2D);

	/*GLfloat anisotropyLevel;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropyLevel);
	printf("Max anisotropy level: %f\n", anisotropyLevel);*/

	if (glfwExtensionSupported("GL_ARB_texture_filter_anisotropic")) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
	}

	// now the texture is being unbound from GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, 0);

	// frees the memory allocated for the texture data
	stbi_image_free(texData);

	return true;
}

bool Texture::LoadTextureA()
{
	// loads texture data from file into char* array
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	// generates texture name and binds it to GL_TEXTURE_2D
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);


	// whenever GL_TEXTURE_2D is passed - the current texture thats bound to it is being worked on (in this case flags are applied to it)
	// no texutre/image is being created/passed yet
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// now the texture(image) is being created and the data is being passed to it (via texData)
	// GL_RGBA is the format of the image (in this case its RGBA, with Alpha)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

	// generates mipmaps for the texture (downscaled versions of the texture
	// for when the texture is further away from the camera (prevents aliasing))
	glGenerateMipmap(GL_TEXTURE_2D);

	if (glfwExtensionSupported("GL_ARB_texture_filter_anisotropic")) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
	}

	// now the texture is being unbound from GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, 0);

	// frees the memory allocated for the texture data
	stbi_image_free(texData);

	return true;
}

void Texture::UseTexture()
{
	// dont understand what this does...
	glActiveTexture(GL_TEXTURE1);
	// binds this object's texture to GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, textureID);
}

// cleanup function
void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}


Texture::~Texture()
{
	ClearTexture();
}
