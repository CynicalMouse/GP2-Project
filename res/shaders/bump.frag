#version 400

in vec2 texCoords;

uniform sampler2D inputTexture;
uniform sampler2D normalT;

uniform vec3 lightDirection;

out vec4 FragColor;

void main()
{
	vec3 normal = texture2D(normalT, texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	
	vec3 colour = texture2D(inputTexture, texCoords).rgb;
	vec3 ambient = 0.8 * colour;
	
	float diff = max(dot(lightDirection, normal), 0.0);
	vec3 inputTexture = diff * colour;

	vec3 viewDir = vec3(0.5,0.5,0.5);
    //vec3 reflectDir = reflect(-lightDirection, normal); for more accurate spec when using TBN
    vec3 halfwayDir = normalize(lightDirection + viewDir); 

	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = vec3(0.2) * spec;
	FragColor = vec4(ambient + inputTexture + specular, 1.0);
}