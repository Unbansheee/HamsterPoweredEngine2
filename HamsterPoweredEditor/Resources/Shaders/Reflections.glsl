

//Calculate reflection on the surface
vec3 GetEnvironmentReflection(vec3 normal, vec3 viewDir, samplerCube envMap, float brightness)
{
    vec3 reflection = reflect(-viewDir, normal);
    vec3 envColor = texture(envMap, reflection).rgb;
    return envColor * brightness;
}