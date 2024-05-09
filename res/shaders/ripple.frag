#version 400

// GOT HELP FROM WINDOWS COPILOT/CHATGPT

in vec2 texCoords;

uniform sampler2D inputTexture;
uniform float time;  // Time for animation
uniform float amplitude; // Strength of animation
uniform float frequency; // Frequency of the ripples

out vec4 FragColor;

void main()
{
    // Calculate ripple effect
    vec2 uv = texCoords;

    uv -= 0.5;  // Center the coordinates around (0,0)
    float radius = length(uv);  // Distance from center

    uv += amplitude * sin(frequency * radius - time);

    vec4 color = texture(inputTexture, uv);

    FragColor = color;
}
