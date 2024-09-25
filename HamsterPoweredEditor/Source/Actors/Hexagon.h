#include "Actor.h"
#include "Core/App.h"
#include "Rendering/GLVertexArray.h"
#include "Rendering/GLBuffers.h"
#include "ResourceManagement/Shader.h"
#include "ResourceManagement/Texture.h"

class Hexagon : public Actor
{
public:
    Hexagon();
    ~Hexagon();

    void Draw() override;
    void Update(Timestep ts) override;
    void Begin() override;
    void OnDestroy() override;

    Texture* SetTexture(const std::string& path);
    void SetTexture(const std::string& path, const std::string& path2);
    void OnInspectorGUI() override;

    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& json) override;

    float vertices[11 * 7] = {
        //Positions             //Vertex Colurs     //Texture Coords    //Normals
        0.0f,  0.0f, 0.0f,       0.f, 0.f, 1.f,      0.5f, 0.5f,        0.0f, 0.0f, -1.f,// center
        -0.25f, 0.5f, 0.0f,      0.f, 1.f, 1.f,      0.25f, 1.0f,       0.0f, 0.0f, -1.f,// LTOP
         0.25f, 0.5f, 0.0f,      1.f, 1.f, 0.f,      0.75f, 1.0f,       0.0f, 0.0f, -1.f,// RTOP
         0.5f, 0.0f, 0.0f,       1.f, 0.f, 1.f,      1.0f, 0.5f,        0.0f, 0.0f, -1.f,// R
         0.25f,-0.5f, 0.0f,      1.f, 0.f, 1.f,      0.75f, 0.0f,       0.0f, 0.0f, -1.f,// RBOT
        -0.25f,-0.5f, 0.0f,      0.f, 0.f, 1.f,      0.25f, 0.0f,       0.0f, 0.0f, -1.f,// LBOT
        -0.5f, 0.0f, 0.0f,       0.f, 0.f, 1.f,      0.0f, 0.5f,        0.0f, 0.0f, -1.f// L
     };

    
    unsigned int indices[18] = {
        2, 1, 0, 
        3, 2, 0, 
        4, 3, 0, 
        5, 4, 0, 
        6, 5, 0, 
        1, 6, 0         
    };  

    uint32_t m_DrawMode = GL_TRIANGLES;
    std::shared_ptr<GLVertexArray> va;
    std::shared_ptr<GLIndexBuffer> ib;
    std::shared_ptr<GLVertexBuffer> vb;
    std::shared_ptr<Shader> shader;
    Texture* texture;
    Texture* texture2;

    std::vector<glm::vec3> m_InstancePositions = {{-2, 0, 0}};
    
};
