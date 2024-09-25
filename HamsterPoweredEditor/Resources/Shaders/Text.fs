#version 460 core

#include "Resources/Shaders/Common.glsl"


in float v_TextureID;





void main()
{
    float Alpha = texture(u_Textures[int(v_TextureID)], v_TexCoord).r;
    //FinalColor = vec4(1, 1, 1, 1);
    if (Wireframe)
    {
        FinalColor= vec4(1, 1, 1, 1);
    }
    else
    {

        if (Alpha < 0.5)
        {
            discard;
        }
        else
        {
            FinalColor = vec4(FragColor, Alpha);
        }
        

        
    }
    
}