#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

// CONSTRUCTOR & DESTRUCTOR //
#pragma region Constructor & Destructor
Texture::Texture()
{
	textureID = NULL;
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

#pragma endregion

// TEXTURE MANAGEMENT //
#pragma region Texture Management
void Texture::initialiseTexture(const std::string& fileName)
{
	textureID = 1;
	int width, height, numComponents;

	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);

	if (imageData == NULL)
	{
		std::cerr << "Texture loading failed for file: " << fileName << std::endl;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

void Texture::initialiseTextureWrapped(const std::string& fileName)
{
	textureID = 1;
	int width, height, numComponents;

	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);

	if (imageData == NULL)
	{
		std::cerr << "Wrapped texture loading failed for file: " << fileName << std::endl;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Wraps texture on x and y axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

void Texture::initialiseNormals(const std::string& fileName)
{
	textureID = 1;
	int width, height, numComponents;
	unsigned char* imageData = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

	if (imageData == NULL)
	{
		std::cerr << "Normal loading failed for file:" << fileName << std::endl;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

void Texture::initialiseDisplacement(const std::string& fileName)
{
	textureID = 1;
	int width, height, numComponents;
	unsigned char* imageData = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

	if (imageData == NULL)
	{
		std::cerr << "Displacement loading failed for file:" << fileName << std::endl;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

void Texture::initialiseCubeMap(std::string faces[])
{
	textureID = 1;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, numComponents;

	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char* imageData = stbi_load(faces[i].c_str(), &width, &height, &numComponents, 3);

		if (imageData != NULL)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			stbi_image_free(imageData);
		}
		else
		{
			std::cerr << "Cubemap texture failed loading for file: " << faces[i] << std::endl;
			stbi_image_free(imageData);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


#pragma endregion

// TEXTURE MANAGEMENT //
#pragma region Texture Management
void Texture::bindTexture(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

#pragma endregion