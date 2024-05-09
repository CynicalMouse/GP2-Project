#version 400

in vec2 texCoords;

uniform sampler2D inputTexture; // The input texture
uniform vec3 addedColor;

out vec4 FragColor;



void main()
{
    vec3 texColor = texture(inputTexture, texCoords).rgb;

    texColor.r += addedColor.r/10;
    texColor.g += addedColor.g/10;
    texColor.b += addedColor.b/10;
    
    FragColor = vec4(texColor, 1.0);
}