#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 TextCoords;
layout (location = 2) in vec3 vertexNormal;

uniform mat4 transform;

out vec3 v_norm;
out vec4 v_pos; 
out vec2 texCoords;

void main()
{
	texCoords = TextCoords;
	v_norm = vertexNormal;
	v_pos = transform * vec4(vertexPosition, 1.0);
	gl_Position = transform * vec4(vertexPosition, 1.0);
}