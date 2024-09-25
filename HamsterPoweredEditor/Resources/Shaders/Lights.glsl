


struct DirLight
{
    vec3 Direction;
    vec3 Color;
    float Intensity;
};

struct PointLight
{
    vec3 Position;
    vec3 Color;
    float Intensity;
    float Radius;
};

uniform PointLight PointLights[8];
uniform int PointLightCount;
uniform DirLight DirLights[8];
uniform int DirLightCount;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Direction);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 HalfwayVector = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, HalfwayVector), 0.0), 128.0f);

    return (diff * light.Color + spec * light.Color) * light.Intensity;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float radius)
{
    vec3 lightDir = normalize(light.Position - fragPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // blinn-phong specular shading
    vec3 HalfwayVector = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, HalfwayVector), 0.0), 128.0f);
    
    // attenuation
    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 - distance / radius;
    attenuation = clamp(attenuation, 0.0, 1.0);


    return (diff * light.Color + spec * light.Color) * light.Intensity * attenuation;

}

