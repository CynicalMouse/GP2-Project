#version 330 core

in vec2 texCoords;

uniform vec3 cameraPos;
uniform vec3 eyePos;
uniform sampler2D inputTexture;
uniform float pupilSize;

out vec4 FragColor;

void main()
{
	vec3 lookDirection = normalize(eyePos - cameraPos);

	vec2 pupilOffset = vec2(lookDirection.x, lookDirection.y);

	vec2 newTexCoords = texCoords + pupilOffset;

	FragColor = texture2D(inputTexture, newTexCoords);
}
