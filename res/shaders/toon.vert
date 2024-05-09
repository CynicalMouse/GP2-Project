#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 TextCoords;
layout (location = 2) in vec3 vertexNormals;

uniform mat4 transform;

out vec3 normal;
out vec2 texCoords;

void main()
{
	texCoords = TextCoords;
	normal = vertexNormals;
	gl_Position = transform * vec4(position, 1.0);
}

