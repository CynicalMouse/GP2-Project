#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 TextCoords;
layout (location = 2) in vec3 vertexNormal;

uniform mat4 transform;

uniform mat4 model;
uniform vec3 cameraPosition;

out vec2 texCoords;
out vec3 normal;
out vec3 viewDirection;

void main()
{
	texCoords = TextCoords;

	vec4 worldPosition = transform * vec4(vertexPosition, 1.0);
	vec4 viewPosition = vec4(worldPosition.xyz - cameraPosition, 1.0);
	gl_Position = transform * vec4(vertexPosition, 1.0);

	normal = mat3(transpose(inverse(model))) * vertexNormal;
	viewDirection = -vec3(viewPosition);
}