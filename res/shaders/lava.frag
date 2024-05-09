#version 400

// GOT HELP FROM WINDOWS COPILOT/CHATGPT

in vec2 texCoords;

uniform sampler2D inputTexture;
uniform float time; 

out vec4 FragColor;

void main()
{
    // Lava flow effect: use noise to distort the texture coordinates
    vec2 distorted_texCoords = texCoords + vec2(sin(texCoords.y * 10.0 + time), cos(texCoords.x * 10.0 + time)) * 0.02;

    vec3 color = texture(inputTexture, distorted_texCoords).rgb;

    FragColor = vec4(color, 1.0);
}
