#pragma once
#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <vector>

class Vertex
{
public:
	// CONSTRUCTOR //
	Vertex(const glm::vec3& pos, const glm::vec2& texturePos)
	{
		this->pos = pos;
		this->texturePos = texturePos;
		this->normal = normal;
	}

	// GETTERS //
	glm::vec3* getPos() { return &pos; }
	glm::vec2* getTexturePos() { return &texturePos; }
	glm::vec3* getNormal() { return &normal; }

protected:

private:
	// ATTRIBUTES //
	glm::vec3 pos;
	glm::vec2 texturePos;
	glm::vec3 normal;
};