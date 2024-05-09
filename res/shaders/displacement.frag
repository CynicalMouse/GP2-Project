#version 400

// BASED ON THE TUTORIAL FOUND HERE: https://learnopengl.com/Advanced-Lighting/Parallax-Mapping

in vec3 FragPos;
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform float heightScale;

out vec4 FragColor;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 24;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  

    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;

    // Depth of current layer
    float currentLayerDepth = 0.0;

    // Calculate coordinate offset per layer
    vec2 textureOffset  = (viewDir.xy / viewDir.z * heightScale) / numLayers;
  
    // Get initial values
    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // Shift texture coordinates
        currentTexCoords -= textureOffset;

        // Get current depth value
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  

        // Get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // Get previous coordinates
    vec2 prevTexCoords = currentTexCoords + textureOffset ;

    // Linear interpolation of texture coordinates
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
    float weight = (currentDepthMapValue - currentLayerDepth) / (currentDepthMapValue - beforeDepth);
    vec2 finalTexCoords = mix(currentTexCoords, prevTexCoords, weight);

    return finalTexCoords;
}

void main()
{           
    // calculate view direction
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);

    // Offset texture coordinates
    vec2 texCoords = ParallaxMapping(TexCoords, viewDir);     
    
    // Discard anything outside of texture boundaries.
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    // Get normal from normal map and normalise it
    vec3 normal = normalize(texture(normalMap, texCoords).rgb * 2.0 - 1.0);  

    // get diffuse color
    vec3 color = texture(diffuseMap, texCoords).rgb;

    // ambient
    vec3 ambient = 0.1 * color;

    // diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // specular    
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;


    FragColor = vec4(ambient + diffuse + specular, 1.0);
}