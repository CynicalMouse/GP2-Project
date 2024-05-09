#version 330 core

in vec2 texCoords;
in vec3 normal;

uniform vec3 lightDirection;
uniform sampler2D inputTexture;

out vec4 FragColor;

void main()
{
	float intensity;
	vec3 color;

    intensity = dot(lightDirection, normal);
	
	color = vec3(intensity, intensity, intensity);

	if (intensity > 0.8)
	{
		color = vec3(0.9,0.8,0.8);
	}
	else if (intensity > 0.6)
	{
		color = vec3(0.6,0.3,0.3);
	}
	else if	(intensity > 0.4)
	{
		color = vec3(0.4,0.2,0.2);
	}
	else if (intensity > 0.2)
	{
		color = vec3(0.3,0.15,0.15);
	}
	else if (intensity > 0.05)
	{
		color = vec3(0.2,0.1,0.1);
	}
	else
	{
		color = vec3(0.1,0.0,0.0);
	}
	

	vec3 textureColor = texture2D(inputTexture, texCoords).rgb;

	FragColor = vec4(color, 1.0);
}
