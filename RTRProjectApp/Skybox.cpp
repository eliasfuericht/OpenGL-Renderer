#include "Skybox.h"

Skybox::Skybox(){}

Skybox::Skybox(std::string folder) {
	faces = {
			"Textures/replace/right.jpg",
			"Textures/replace/left.jpg",
			"Textures/replace/top.jpg",
			"Textures/replace/bottom.jpg",
			"Textures/replace/front.jpg",
			"Textures/replace/back.jpg"
	};

	// Replace "replace" with the provided folder path
	for (std::string& face : faces) {
		size_t found = face.find("replace");
		if (found != std::string::npos) {
			face.replace(found, 7, folder);
		}
	}

	cubemapTexture = LoadCubemap();
}

unsigned int Skybox::LoadCubemap()
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void Skybox::RenderSkybox(GLuint projectionLocation, glm::mat4 projection, GLuint viewLocation, glm::mat4 view, GLuint modelLocation)
{
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 model(1.0f);

	model = glm::mat4(1.0f);

	model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
}

