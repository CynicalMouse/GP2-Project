#version 400

// GOT HELP FROM WINDOWS COPILOT/CHATGPT

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TextCoords;

uniform mat4 transform;

out vec2 texCoords;

void main()
{
    gl_Position = transform * vec4(VertexPosition, 1.0);
    texCoords = TextCoords;
}
