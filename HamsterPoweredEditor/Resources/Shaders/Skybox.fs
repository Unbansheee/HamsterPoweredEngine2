#version 460 core
#include "Resources/Shaders/Common.glsl"

//uniform float u_SkyboxBrightness;
in vec3 v_Coord;

void main()
{
    vec3 col = texture(u_Skybox, v_Coord).xyz;
    
    // Apply exposure
    col = 1.0 - exp(-col * u_SkyboxBrightness);
    
    FinalColor = vec4(col, 1.0);

}