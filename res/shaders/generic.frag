#version 400

in vec2 texCoord;

uniform sampler2D inputTexture;

out vec4 FragColor;

void main()
{
	FragColor = texture(inputTexture, texCoord);
}