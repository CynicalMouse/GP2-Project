#version 400

in vec2 texCoords;
in vec4 v_pos; 

uniform sampler2D inputTexture;

out vec4 FragColor;

vec3 fogColor = vec3(0.0, 0.0, 0.0); // fade to black


float maxDist = 10.0f; //fog max distance
float minDist = 0.0f; //fog min distance

void main() 
{
	float dist = abs( v_pos.z );

	float fogFactor = (maxDist - dist)/(maxDist - minDist);
	fogFactor = clamp( fogFactor, 0.0, 1.0 );

	vec3 lightColor = vec3(1.0,1.0,1.0);

	// Old code for when fog went to a color like black, now it goes transparent
	vec3 color = mix( fogColor, lightColor, fogFactor);

	vec3 textureColor = texture2D(inputTexture, texCoords).rgb;

	FragColor = vec4(color * textureColor, fogFactor);
}
