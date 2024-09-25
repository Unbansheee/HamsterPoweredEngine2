#pragma once
#include <functional>
#include <queue>
#include <glad/glad.h>

#include "OrthographicCamera.h"

#include "ResourceManagement/Shader.h"
#include "GLFrameBuffer.h"
#include "GLVertexArray.h"
#include "Lights.h"
#include "UI/RendererSettings.h"

class Texture;

struct RenderSettings
{
    uint32_t DrawMode = GL_TRIANGLES;
    bool Blending = false;
    bool DepthTest = true;
    bool Culling = true;
    bool DepthWrite = true;
};

struct RenderObject
{
    const std::shared_ptr<Shader>& shader;
    const std::shared_ptr<GLVertexArray>& vertexArray;
    const glm::mat4 transform;
    const RenderSettings settings;
    std::vector<Texture*> textures;
};

class Renderer
{
public:


    
    enum class RenderMode
    {
        UNLIT,
        WIREFRAME,
        LIT
    };
    
    friend class OpenGLRendererAPI;
    friend class RendererSettings;
    friend class RendererAPI;
    friend class Viewport;
    friend class Stats;
    friend class Scene;
    
    static void Init();
    static void BeginScene(View& camera);
    static void EndScene();
    static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<GLVertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f), const std::vector<Texture*>& textures = {}, const RenderSettings& settings = RenderSettings());
    static void Submit(const PointLightData& light);
    static void Submit(const DirectionalLightData& light);
    static void SetClearColor(const glm::vec4& color);
    static const glm::vec4& GetClearColor() {return clearColor;}
    static double AspectRatio();
    static unsigned int GetRenderTexture() {return renderTexture;}
    static void SetRenderMode(RenderMode mode);
    static RenderMode GetRenderMode() {return m_renderMode;}
    static glm::vec2 GetViewportSize() {return {m_Width, m_Height};}
    static glm::vec2 GetViewportPosition() {return m_viewportPosition;}
    static void SetMSAASamples(int samples);
    static int GetMSAASamples() {return m_MSAASamples;}
    static void Render();
    static void DeferredUpdate(); // Called after the scene is rendered and the texture is shown on the screen
    static glm::mat4& GetProjectionMatrix() {return m_ProjectionMatrix;}
    static glm::mat4& GetViewMatrix() {return m_ViewMatrix;}
    static glm::vec3 ScreenToWorldPos(glm::vec2 screenPos, float distanceBetweenPlanes);
private:
    static void Clear();
    
    static void DrawIndexed(const std::shared_ptr<GLVertexArray>& vertexArray, RenderSettings settings);
    static void Resize(int width, int height);

    
private:
    inline static unsigned int frameBuffer;
    inline static unsigned int depthrenderbuffer;
    inline static unsigned int renderTexture;
    inline static std::shared_ptr<GLFrameBuffer> m_FrameBuffer;
    inline static GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    inline static int drawCalls;
    inline static int m_Width;
    inline static int m_Height;
    inline static glm::vec4 clearColor;
    inline static RenderMode m_renderMode;
    inline static std::shared_ptr<Shader> m_WireShader;
    inline static int m_MSAASamples = 16;

    inline static std::vector<RenderObject> m_RenderObjects;
    inline static std::vector<PointLightData> m_PointLights;
    inline static std::vector<DirectionalLightData> m_DirectionalLights;

    inline static float m_AmbientLightStrength = 0.2f;
    inline static glm::vec2 m_viewportPosition;
    inline static std::queue<std::function<void()>> m_DeferredTasks;
    
    struct SceneData
    {
        glm::mat4 ViewProjectionMatrix;
        glm::vec3 CameraPosition;
    };

    static SceneData* m_SceneData;
    inline static glm::mat4 m_ProjectionMatrix;
    inline static glm::mat4 m_ViewMatrix;


};

