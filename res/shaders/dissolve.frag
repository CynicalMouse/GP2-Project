#version 400

in vec2 texCoords;

uniform sampler2D inputTexture;
uniform float time;

out vec4 FragColor;

void main()
{
    vec3 color = texture(inputTexture, texCoords).rgb;

    float alpha = 1.0 - time;
    alpha = clamp(alpha, 0.0, 1.0);

    // Output the final color
    FragColor = vec4(color, alpha);
}