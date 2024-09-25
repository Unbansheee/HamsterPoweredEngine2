#include "Renderer.h"

//#include <glm/ext/matrix_projection.hpp>

#include <glm/ext/matrix_projection.hpp>

#include "Actors/MeshActor.h"
#include "Actors/SkyboxActor.h"
#include "Core/App.h"
#include "Core/Raycast.h"
#include "Core/Timer.h"
#include "ResourceManagement/Texture.h"
#include "ResourceManagement/Shader.h"

Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

void Renderer::Init()
{
    
    m_Width = 1280;
    m_Height = 720;

    m_WireShader= Shader::Create("Resources/Shaders/VertexColor.vs", "Resources/Shaders/Wireframe.fs");
    
    FrameBufferSpecification spec;
    spec.Width = m_Width;
    spec.Height = m_Height;
    spec.Samples = m_MSAASamples;
    m_FrameBuffer.reset(new GLFrameBuffer(spec));

    glEnable(GL_MULTISAMPLE);
}

void Renderer::BeginScene(View& camera)
{
    Clear();
    if (!m_FrameBuffer->IsValid()) m_DeferredTasks.push([](){m_FrameBuffer->Invalidate();});
    m_FrameBuffer->BindMSAABuffer();
    m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    m_SceneData->CameraPosition = camera.GetPosition();
    m_ProjectionMatrix = camera.GetProjectionMatrix();
    m_ViewMatrix = camera.GetViewMatrix();
    drawCalls = 0;
}

void Renderer::EndScene()
{
    m_FrameBuffer->Unbind();
    m_PointLights.clear();
    m_DirectionalLights.clear();
    m_RenderObjects.clear();
}

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<GLVertexArray>& vertexArray, const glm::mat4& transform, const std::vector<Texture*>& textures, const RenderSettings& settings)
{
    m_RenderObjects.push_back({ shader, vertexArray, transform, settings, textures });
 
}

void Renderer::Submit(const PointLightData& light)
{
    m_PointLights.push_back(light);
}

void Renderer::Submit(const DirectionalLightData& light)
{
    m_DirectionalLights.push_back(light);
}

void Renderer::SetRenderMode(RenderMode mode)
{
    m_renderMode = mode;
    switch (mode)
    {
    case RenderMode::WIREFRAME:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_LIGHTING);
        break;
    case RenderMode::UNLIT:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_LIGHTING);
        break;
    case RenderMode::LIT:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);
        break;
    }
}

void Renderer::SetMSAASamples(int samples)
{
    m_MSAASamples = samples;
    m_DeferredTasks.push([]()
    {
        FrameBufferSpecification spec;
        spec.Width = m_Width;
        spec.Height = m_Height;
        spec.Samples = m_MSAASamples;
        m_FrameBuffer.reset(new GLFrameBuffer(spec));
    });
    
}

void Renderer::Render()
{
    std::map < std::shared_ptr<Shader>, std::vector<RenderObject>> renderMap;
    for (auto& object : m_RenderObjects)
    {
        renderMap[object.shader].push_back(object);
    }

    for (auto& [shader, objects] : renderMap)
    {
        shader->Bind();

        for (auto object : objects)
        {
            auto& vertexArray = object.vertexArray;
            auto& transform = object.transform;
            auto& settings = object.settings;
            
                   
            vertexArray->Bind();
            for (int i = 0; i < object.textures.size(); i++)
            {
                if (object.textures[i] != nullptr)
                {
                    object.textures[i]->Bind(i);
                    shader->SetUniform1i("u_Textures[" + std::to_string(i) + "]", i);
                }
                else
                {
                    Texture* tex = Texture::GetWhiteTexture();
                    tex->Bind(i);
                    shader->SetUniform1i("u_Textures[" + std::to_string(i) + "]", i);
                }
            }
            
            shader->SetUniform1i("Wireframe", m_renderMode == RenderMode::WIREFRAME);
            shader->SetUniform1i("Unlit", m_renderMode == RenderMode::UNLIT);
            
            shader->SetUniform1f("AmbientStrength", m_AmbientLightStrength);
            shader->SetUniform3f("AmbientColour", clearColor.x, clearColor.y, clearColor.z);

            glm::vec3 cameraPos = m_SceneData->CameraPosition;
            shader->SetUniform3f("CameraPosition", cameraPos.x, cameraPos.y, cameraPos.z);
            
            shader->SetUniformMat4f("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
            shader->SetUniformMat4f("u_Transform", transform);
            shader->SetUniform1f("CurrentTime", App::Instance().GetTime());

            for (int i = 0; i < m_PointLights.size() && i < 8; i++)
            {
                shader->SetUniform3f("PointLights[" + std::to_string(i) + "].Position", m_PointLights[i].position.x, m_PointLights[i].position.y, m_PointLights[i].position.z);
                shader->SetUniform3f("PointLights[" + std::to_string(i) + "].Color", m_PointLights[i].color.x, m_PointLights[i].color.y, m_PointLights[i].color.z);
                shader->SetUniform1f("PointLights[" + std::to_string(i) + "].Intensity", m_PointLights[i].intensity);
                shader->SetUniform1f("PointLights[" + std::to_string(i) + "].Radius", m_PointLights[i].radius);
            }
            shader->SetUniform1i("PointLightCount", (int)m_PointLights.size());

            for (int i = 0; i < m_DirectionalLights.size() && i < 8; i++)
            {
                shader->SetUniform3f("DirLights[" + std::to_string(i) + "].Direction", m_DirectionalLights[i].direction.x, m_DirectionalLights[i].direction.y, m_DirectionalLights[i].direction.z);
                shader->SetUniform3f("DirLights[" + std::to_string(i) + "].Color", m_DirectionalLights[i].color.x, m_DirectionalLights[i].color.y, m_DirectionalLights[i].color.z);
                shader->SetUniform1f("DirLights[" + std::to_string(i) + "].Intensity", m_DirectionalLights[i].intensity);
            }
            shader->SetUniform1i("DirLightCount", (int)m_DirectionalLights.size());

            auto skybox =  App::Instance().m_currentScene->GetActorOfClass<SkyboxActor>();
            if (skybox)
            {
                if (skybox->cubemap) skybox->cubemap->Bind(32);
                shader->SetUniform1i("u_Skybox", 32);
                shader->SetUniform1f("u_SkyboxBrightness", skybox->brightness);
            }
            
                
            DrawIndexed(vertexArray, settings);
            vertexArray->Unbind();
            drawCalls++;
        }
    }
    
    
    m_FrameBuffer->BlitToScreen();
}

void Renderer::DeferredUpdate()
{
    while (!m_DeferredTasks.empty())
    {
        m_DeferredTasks.front()();
        m_DeferredTasks.pop();
    }
}

glm::vec3 Renderer::ScreenToWorldPos(glm::vec2 screenPos, float distanceBetweenPlanes)
{
    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, m_Width, m_Height);
    
    //get mouse coords
    glm::vec2 mousePos = screenPos;
    mousePos.y = m_Height - mousePos.y;

    glm::vec3 v0 = glm::unProject(glm::vec3(float(mousePos.x), float(mousePos.y), distanceBetweenPlanes),m_ViewMatrix,m_ProjectionMatrix, viewport);
    
    return v0;
    
}

void Renderer::Clear()
{
    Renderer::m_FrameBuffer->BindMSAABuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Renderer::m_FrameBuffer->Unbind();
}

void Renderer::SetClearColor(const glm::vec4& color)
{
    clearColor = color;
    glClearColor(color.r, color.g, color.b, color.a);
}

double Renderer::AspectRatio()
{
    return GetViewportSize().x / GetViewportSize().y;
}

void Renderer::DrawIndexed(const std::shared_ptr<GLVertexArray>& vertexArray, RenderSettings settings)
{
    Renderer::m_FrameBuffer->BindMSAABuffer();
    

        if (settings.Blending)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }

        if (settings.Culling)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        
        }

        if (settings.DepthTest)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        if (settings.DepthWrite)
        {
            glEnable(GL_DEPTH_WRITEMASK);
        }
        else
        {
            glEnable(GL_DEPTH_WRITEMASK);
        }

    int count = vertexArray->GetIndexBuffer()->GetCount();
    glDrawElements(settings.DrawMode, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    Renderer::m_FrameBuffer->Unbind();

}


void Renderer::Resize(int width, int height)
{
    m_Width = width;
    m_Height = height;

    m_DeferredTasks.push([=]()
    {
        FrameBufferSpecification spec;
        spec.Width = m_Width;
        spec.Height = m_Height;
        spec.Samples = m_MSAASamples;
        m_FrameBuffer.reset(new GLFrameBuffer(spec));
    });


}



