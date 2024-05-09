#version 400

// GOT HELP FROM WINDOWS COPILOT/CHATGPT

layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoords;

void main()
{
    vec3 viewDir = mat3(view) * position;

    TexCoords = position;

    gl_Position = projection * vec4(viewDir, 1.0);
}