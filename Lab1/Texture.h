#pragma once
#include <string>
#include <GL\glew.h>
#include <vector>

class Texture
{

public:
	// CONSTRUCTOR & DESTRUCTOR //
	Texture();
	~Texture();

	// TEXTURE INITIALIZATION //
	void initialiseTexture(const std::string& fileName);
	void initialiseTextureWrapped(const std::string& fileName);
	void initialiseNormals(const std::string& fileName);
	void initialiseDisplacement(const std::string& fileName);
	void initialiseCubeMap(std::string face[]);

	// TEXTURE MANAGEMENT //
	void bindTexture(unsigned int unit); // bind upto 32 textures
	GLuint getID() { return textureID; }

private:
	GLuint textureID;
};