#pragma once
#include <string>
#include <GL\glew.h>
#include "Transform.h"

class Shader
{
public:
	// CONTRUCTOR & DESTRUCTOR //
	Shader();
	~Shader();

	// SHADER MANAGEMENT //
	void initialiseShader(const std::string& filename);
	std::string loadShader(const std::string& fileName);
	GLuint createShader(const std::string& text, unsigned int type);
	void updateShader(const Transform& transform, const Camera& camera);
	void bindShader();

	GLuint getID() { return shaderID; }

	// ERROR CHECKING //
	void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	void checkUniformLocation(const std::string& name);

	// SETTERS //
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setVec2(const std::string& name, const glm::vec2& value);
	void setVec2(const std::string& name, float x, float y);
	void setVec3(const std::string& name, const glm::vec3& value);
	void setVec3(const std::string& name, float x, float y, float z);
	void setVec4(const std::string& name, const glm::vec4& value);
	void setVec4(const std::string& name, float x, float y, float z, float w);
	void setMat2(const std::string& name, const glm::mat2& mat);
	void setMat3(const std::string& name, const glm::mat3& mat);
	void setMat4(const std::string& name, const glm::mat4& mat);

protected:

private:
	Shader(const Shader& other) {}

	void operator=(const Shader& other) {}

	// ATTRIBUTES //
	enum
	{
		TRANSFORM_U,
		NUM_UNIFORMS
	};

	static const unsigned int shaderCount = 2;
	GLuint shaderID; // Track the shader program
	GLuint shaders[shaderCount];
	GLuint uniforms[NUM_UNIFORMS];
};
