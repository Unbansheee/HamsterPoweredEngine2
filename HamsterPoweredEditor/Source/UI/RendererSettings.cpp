#include "RendererSettings.h"

#include "Core/App.h"
#include "Rendering/Renderer.h"

void RendererSettings::Init()
{
    UIComponent::Init();
    name = "Renderer Settings";
}

void RendererSettings::Begin()
{
    UIComponent::Begin();
}

void RendererSettings::Update(Timestep ts)
{
    UIComponent::Update(ts);
    glm::vec4 colour = Renderer::GetClearColor();
    if (ImGui::ColorEdit4("Scene Colour", &colour.r))
    {
        App::Instance().m_currentScene->SetColour(colour);
    }
    ImGui::DragFloat("Ambient Strength", &Renderer::m_AmbientLightStrength, 0.1f, 0.0f);

    
    int rendermode = (int)Renderer::GetRenderMode();
    if (ImGui::Combo("Render Mode", &rendermode, "Unlit\0Wireframe\0Lit\0"))
    {
        Renderer::SetRenderMode((Renderer::RenderMode)rendermode);
    }

    int samples = Renderer::GetMSAASamples();
    if (ImGui::DragInt("MSAA Samples", &samples, 1, 0, 16))
    {
        Renderer::SetMSAASamples(samples);
    }
    
}

void RendererSettings::End()
{
    UIComponent::End();
}
