#include "SkyboxActor.h"

#include "ResourceManagement/Cubemap.h"
#include "ResourceManagement/Texture.h"
#include "Core/Scene.h"
#include "UI/HPImGui.h"

SkyboxActor::SkyboxActor()
{
    cubemap.reset(new Cubemap(path));
    
    SetScale(100000.000);
    mesh.Cube();
    auto shader = Shader::Create("Resources/Shaders/Skybox.vs", "Resources/Shaders/Skybox.fs");
    mesh.SetShader(shader);
}

void SkyboxActor::FixedUpdate(double ts)
{
    Actor::FixedUpdate(ts);
}

void SkyboxActor::Update(Timestep ts)
{
    Actor::Update(ts);
}

void SkyboxActor::Draw()
{
    Actor::Draw();
    //mesh.Draw(m_transform);

    GLint OldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
    GLint OldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    
    mesh.GetShader()->Bind();
    
    mesh.GetShader()->SetUniformMat4f("u_ViewProjection", m_scene->GetCameraController()->GetCamera()->GetViewProjectionMatrix());
    mesh.GetShader()->SetUniformMat4f("u_Transform", m_transform);
    mesh.GetShader()->SetUniform1i("u_Skybox", 32);
    mesh.GetShader()->SetUniform1f("u_SkyboxBrightness", brightness);
    if (cubemap) cubemap->Bind(32);
    mesh.GetVAO()->Bind();
    glDrawElements(GL_TRIANGLES, mesh.GetIndices().size(), GL_UNSIGNED_INT, nullptr);
    mesh.GetVAO()->Unbind();
    if (cubemap) cubemap->Unbind();
    mesh.GetShader()->Unbind();

    glCullFace(OldCullFaceMode);
    glDepthFunc(OldDepthFuncMode);
    
}

void SkyboxActor::Begin()
{
    Actor::Begin();

    
}

void SkyboxActor::OnDestroy()
{
    Actor::OnDestroy();

}

nlohmann::json SkyboxActor::Serialize()
{
    nlohmann::json j = Actor::Serialize();
    j["Path"] = path;
    j["Brightness"] = brightness;
    
    return j;
}

void SkyboxActor::Deserialize(nlohmann::json& j)
{
    Actor::Deserialize(j);
    if (j.contains(("Path")))
    {
        path = j["Path"];
        cubemap.reset(new Cubemap(path));
    }
    if (j.contains("Brightness"))
    {
        brightness = j["Brightness"];
    }
}

void SkyboxActor::OnInspectorGUI()
{
    Actor::OnInspectorGUI();
    if (ImGui::OpenFilePath("Path##Cubemap", path, "Load Texture", "Image File (*.png;*.jpg;*.jpeg){.png,.jpg,.jpeg},.*", "Resources/Textures/Skybox"))
    {
        cubemap.reset(new Cubemap(path));
    }
    ImGui::DragFloat("Brightness", &brightness, 0.01f, 0.0f, 10.0f);
}

SkyboxActor::~SkyboxActor()
{
}
