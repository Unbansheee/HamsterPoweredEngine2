#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "Rendering/GLVertexArray.h"
#include "Rendering/GLBuffers.h"
#include "Actor.h"
#include "Rendering/Renderer.h"
#include "ResourceManagement/Shader.h"
#include "ResourceManagement/Texture.h"

class Quad : public Actor
{
public:
    Quad();
    ~Quad();

    void OnInspectorGUI() override;
    
    void Draw() override;
    void Update(Timestep ts) override;
    void Begin() override;
    void OnDestroy() override;

    virtual Texture* SetTexture(const std::string& path);
    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;
protected:

public:
    float vertices[11 * 4] = {
        //Positions             //Vertex Colurs     //Texture Coords    //Normals
       -0.5f,  0.5f, 0.0f,      0.f, 0.f, 1.f,      0.0f, 1.0f,     0.0f, 0.0f, -1.0f,// top left
       -0.5f, -0.5f, 0.0f,      0.f, 1.f, 1.f,      0.0f, 0.0f,     0.0f, 0.0f, -1.0f,// bottom left
        0.5f, -0.5f, 0.0f,      1.f, 1.f, 0.f,      1.0f, 0.0f,     0.0f, 0.0f, -1.0f,// bottom right
        0.5f,  0.5f, 0.0f,      1.f, 0.f, 1.f,      1.0f, 1.0f,     0.0f, 0.0f, -1.0f // top right
    };
    unsigned int indices[3 * 2] = {
        0, 1, 2,   // first triangle
        0, 2, 3    // second triangle
    };  

    uint32_t m_DrawMode = GL_TRIANGLES;
    std::shared_ptr<GLVertexArray> va;
    std::shared_ptr<GLIndexBuffer> ib;
    std::shared_ptr<GLVertexBuffer> vb;
    std::shared_ptr<Shader> shader;
    Texture* texture;
    Texture* texture2;
    std::string m_texturePath;
};
