#version 460 core
#include "Resources/Shaders/Common.glsl"

uniform vec2 UVTransform;
uniform vec2 UVScale;
uniform sampler2D Texture0;

vec3 hueShift(vec3 color, float hue) {
    const vec3 k = vec3(0.57735, 0.57735, 0.57735);
    float cosAngle = cos(hue);
    return vec3(color * cosAngle + cross(k, color) * sin(hue) + k * dot(k, color) * (1.0 - cosAngle));
}

void main()
{
    if (Wireframe)
    {
        FinalColor = vec4(1, 1, 1, 1);
    }
    else
    {
        vec2 uv =  v_TexCoord / UVScale + UVTransform;
        vec4 col = texture(Texture0, uv);
        if (col.a <= AlphaThreshold) {
            discard;
        }

        FinalColor = col;
    }

}


