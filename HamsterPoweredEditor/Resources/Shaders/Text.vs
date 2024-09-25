#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 TexCoord;
layout (location = 3) in float TextureID;

out vec2 v_TexCoord;
out float v_TextureID;
out vec3 FragColor;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

uniform mat4 ProjectionMat;

void main()
{

    mat4 MVP = ProjectionMat * u_Transform;
    gl_Position = MVP * vec4(Position, 1.0f);


    v_TexCoord = TexCoord;
    v_TextureID = TextureID;
    FragColor = Color;
}