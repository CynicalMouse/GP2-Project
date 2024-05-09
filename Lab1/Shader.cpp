#include "Shader.h"
#include <iostream>
#include <fstream>

// CONSTRUCTOR & DESTRUCTOR //
#pragma region Constructor & Destructor
Shader::Shader()
{
	shaderID = NULL;
	shaders[0] = NULL;
	uniforms[0] = NULL;
}

Shader::~Shader()
{
	for (int i = 0; i < shaderCount; i++)
	{
		glDetachShader(shaderID, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	glDeleteProgram(shaderID);
}

#pragma endregion

// SHADER MANAGEMENT //
#pragma region Shader Management

void Shader::bindShader()
{
	glUseProgram(shaderID);
}

void Shader::initialiseShader(const std::string& fileName)
{
	shaderID = glCreateProgram();

	shaders[0] = createShader(loadShader(fileName + ".vert"), GL_VERTEX_SHADER);
	shaders[1] = createShader(loadShader(fileName + ".frag"), GL_FRAGMENT_SHADER);


	for (int i = 0; i < shaderCount; i++)
	{
		glAttachShader(shaderID, shaders[i]);
	}

	glBindAttribLocation(shaderID, 0, "position");
	glBindAttribLocation(shaderID, 1, "textureCoord");
	glBindAttribLocation(shaderID, 2, "VertexNormal");

	glLinkProgram(shaderID);
	checkShaderError(shaderID, GL_LINK_STATUS, true, "Error: Program failed to link");

	glValidateProgram(shaderID);
	checkShaderError(shaderID, GL_VALIDATE_STATUS, true, "Error: Program failed to validate");

	uniforms[TRANSFORM_U] = glGetUniformLocation(shaderID, "transform");
}

GLuint Shader::createShader(const std::string& text, unsigned int type)
{

	GLuint shader = glCreateShader(type);

	if (shader == 0)
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* source = text.c_str();
	GLint length = static_cast<GLint>(text.length());

	glShaderSource(shader, 1, &source, &length);
	glCompileShader(shader);

	checkShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader");

	return shader;
}

std::string Shader::loadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::updateShader(const Transform& transform, const Camera& camera)
{
	glm::mat4 mvp = camera.GetViewProjection() * transform.getModel();
	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GLU_FALSE, &mvp[0][0]);

}

#pragma endregion

// ERROR CHECKING //
#pragma region Error Checking

void Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

void Shader::checkUniformLocation(const std::string& name)
{
	if (glGetUniformLocation(shaderID, name.c_str()) == -1)
	{
		std::cerr << "Unable to load shader uniform: " << name.c_str() << std::endl;
	}
}

#pragma endregion

// SETTERS
#pragma region Setters
// BOOL //

void Shader::setBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
	checkUniformLocation(name);
}

// NUMBERS //

void Shader::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
	checkUniformLocation(name);
}

void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
	checkUniformLocation(name);
}

// VEC2 //

void Shader::setVec2(const std::string& name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
	checkUniformLocation(name);
}

void Shader::setVec2(const std::string& name, float x, float y)
{
	glUniform2f(glGetUniformLocation(shaderID, name.c_str()), x, y);
	checkUniformLocation(name);
}

// VEC3 //

void Shader::setVec3(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
	checkUniformLocation(name);
}

void Shader::setVec3(const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(shaderID, name.c_str()), x, y, z);
	checkUniformLocation(name);
}

// VEC4 //

void Shader::setVec4(const std::string& name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
	checkUniformLocation(name);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(shaderID, name.c_str()), x, y, z, w);
	checkUniformLocation(name);
}

// MATs //

void Shader::setMat2(const std::string& name, const glm::mat2& mat)
{
	glUniformMatrix2fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	checkUniformLocation(name);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat)
{
	glUniformMatrix3fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	checkUniformLocation(name);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	checkUniformLocation(name);
}
#pragma endregion