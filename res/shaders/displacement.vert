#version 400

// BASED ON THE TUTORIAL FOUND HERE: https://learnopengl.com/Advanced-Lighting/Parallax-Mapping

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));   
    TexCoords = aTexCoords;   
    
    // Create TBN matrix using tangent, bitangent and normal
    // Used to displace texture coordinates to create the fake 3D effect and transforming vectors into tangent space
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);
    mat3 TBN = transpose(mat3(T, B, N));

    // Transform light and view positions into tangent space
    // Allows accurate lighting on the fake 3D parts of texture
    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}