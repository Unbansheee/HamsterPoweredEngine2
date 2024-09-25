#version 460 core

#include "Resources/Shaders/Common.glsl"
#include "Resources/Shaders/Lights.glsl"
#include "Resources/Shaders/Reflections.glsl"


void main()
{
    vec4 texColor = texture(u_Textures[0], v_TexCoord);

    vec4 mixed = texColor;
    
    if (mixed.a <= AlphaThreshold && !Wireframe) {
        discard;
    }
    
    if (Wireframe)
    {
        FinalColor = vec4(1, 1, 1, 1);
    }
    else if (Unlit)
    {
        FinalColor = vec4(mixed);
    }
    else
    {
        vec3 ambient = AmbientColour * AmbientStrength;
        vec3 normal = normalize(v_TransformedNormal);
        vec3 viewDir = normalize(CameraPosition - v_Position);
        
        
        vec3 DirectionalLight = vec3(0, 0, 0);
        vec3 PointLight = vec3(0, 0, 0);
        
        for (int i = 0; i < DirLightCount; i++)
        {
            DirectionalLight += CalcDirLight(DirLights[i], normal, viewDir);
        }
        
        
        for (int i = 0; i < PointLightCount; i++)
        {
            PointLight += CalcPointLight(PointLights[i], normal, v_Position, viewDir, PointLights[i].Radius);
        }
        
        
        vec3 result = (ambient + DirectionalLight + PointLight) * mixed.rgb;

        vec3 reflection = GetEnvironmentReflection(normal, viewDir, u_Skybox, u_SkyboxBrightness);
        reflection = 1.0 - exp(-reflection * u_SkyboxBrightness);
        
        
        
        result += reflection * u_Metallic;
        
        

        FinalColor = vec4(result, mixed.a);
    }
    
}


