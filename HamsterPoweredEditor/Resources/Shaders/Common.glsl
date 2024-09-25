
in vec3 FragColor;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_Position;
in vec3 v_TransformedNormal;
in int v_TexIndex;

uniform float CurrentTime;
uniform float AlphaThreshold = 0.25f;
uniform sampler2D u_Textures[32];
uniform samplerCube u_Skybox;
uniform bool Wireframe = false;
uniform bool Unlit = false;
uniform float u_Roughness = 0.5f;
uniform float u_Metallic = 0.1f;
uniform float u_SkyboxBrightness = 1.0f;

uniform vec3 AmbientColour;
uniform float AmbientStrength;
uniform vec3 CameraPosition;

out vec4 FinalColor;