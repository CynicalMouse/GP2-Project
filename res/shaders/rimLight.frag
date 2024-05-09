#version 400

in vec2 texCoords;
in vec3 normal;
in vec3 viewDirection;

//uniform vec3 rimColor;
uniform float intensity;

uniform sampler2D inputTexture;

out vec4 FragColor;

void main()
{
	float rimFactor = max(dot(normalize(normal), normalize(viewDirection)), 0.0);
	rimFactor = pow(1.0 - rimFactor, intensity);

	vec3 color = texture2D(inputTexture, texCoords).rgb;

	FragColor = vec4(color * rimFactor, 1.0);
}