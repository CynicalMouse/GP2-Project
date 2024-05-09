#version 330 core

// USED CHATGPT VIA WINDOWS COPILOT

in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D inputTexture; // The input texture
uniform float pixelSize;

void main()
{
    // Calculate the pixelated texture coordinates
    vec2 pixelatedTexCoords = floor(texCoords * pixelSize) / pixelSize;

    // Sample the texture at the pixelated coordinates
    vec4 texColor = texture(inputTexture, pixelatedTexCoords);

    FragColor = texColor;
}