#pragma once
#include <assimp/scene.h>

#include "Actor.h"

class Texture;

class MeshActor : public Actor
{
public:
    MeshActor();
    ~MeshActor();

    void OnInspectorGUI() override;
    
    void Draw() override;
    void Update(Timestep ts) override;
    void Begin() override;
    void OnDestroy() override;
    
    virtual Texture* SetTexture(const std::string& path, int slot = 0);
    void SetMesh(float* _vertices, uint32_t _size, uint32_t* _indices, uint32_t _isize);

    void LoadMesh(const std::string& path);
    const aiScene* scene;

    void Deserialize(nlohmann::json& j) override;
    nlohmann::json Serialize() override;
    
protected:
    void CopyNodesWithMeshes(aiNode* node, aiMatrix4x4 accTransform);

public:
    //Default mesh is a cube
    std::vector<float> vertices = {
        //Positions             //Vertex Colurs     //Texture Coords    //Normals   
        //Front Quad
        -0.5f,  0.5f, 0.5f,      0.f, 0.f, 1.f,      0.0f, 1.0f,        0.0f, 0.0f,  1.0f, // top left
        -0.5f, -0.5f, 0.5f,      0.f, 1.f, 1.f,      0.0f, 0.0f,        0.0f, 0.0f,  1.0f, // bottom left
         0.5f, -0.5f, 0.5f,      1.f, 1.f, 0.f,      1.0f, 0.0f,        0.0f, 0.0f,  1.0f, // bottom right
         0.5f,  0.5f, 0.5f,      1.f, 0.f, 1.f,      1.0f, 1.0f,        0.0f, 0.0f,  1.0f, // top right

        //Back Quad    
         0.5f,  0.5f, -0.5f,     1.f, 0.f, 0.f,      0.0f, 1.0f,        0.0f, 0.0f,  -1.0f, // top left
         0.5f, -0.5f, -0.5f,     1.f, 1.f, 0.f,      0.0f, 0.0f,        0.0f, 0.0f,  -1.0f, // bottom left
        -0.5f, -0.5f, -0.5f,     0.f, 1.f, 1.f,      1.0f, 0.0f,        0.0f, 0.0f,  -1.0f, // bottom right
        -0.5f,  0.5f, -0.5f,     0.f, 0.f, 1.f,      1.0f, 1.0f,        0.0f, 0.0f,  -1.0f, // top right
 
        //Right Quad        
        0.5f,  0.5f,  0.5f,     1.f, 0.f, 1.f,      0.0f, 1.0f,         1.0f, 0.0f, 0.0f, // top left
        0.5f, -0.5f,  0.5f,     1.f, 1.f, 0.f,      0.0f, 0.0f,         1.0f, 0.0f, 0.0f, // bottom left
        0.5f, -0.5f, -0.5f,     1.f, 1.f, 0.f,      1.0f, 0.0f,         1.0f, 0.0f, 0.0f, // bottom right
        0.5f,  0.5f, -0.5f,     1.f, 0.f, 0.f,      1.0f, 1.0f,         1.0f, 0.0f, 0.0f, // top right

        //Left Quad     
        -0.5f,  0.5f, -0.5f,    0.f, 0.f, 1.f,      0.0f, 1.0f,         -1.0f, 0.0f, 0.0f, // top left
        -0.5f, -0.5f, -0.5f,    0.f, 1.f, 1.f,      0.0f, 0.0f,         -1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, -0.5f,  0.5f,    0.f, 1.f, 1.f,      1.0f, 0.0f,         -1.0f, 0.0f, 0.0f, // bottom right
        -0.5f,  0.5f,  0.5f,    0.f, 0.f, 1.f,      1.0f, 1.0f,         -1.0f, 0.0f, 0.0f, // top right

        //Top Quad      
        -0.5f,  0.5f, -0.5f,    0.f, 0.f, 1.f,      0.0f, 1.0f,         0.0f, 1.0f, 0.0f, // top left
        -0.5f,  0.5f,  0.5f,    0.f, 0.f, 1.f,      0.0f, 0.0f,         0.0f, 1.0f, 0.0f, // bottom left
         0.5f,  0.5f,  0.5f,    1.f, 0.f, 1.f,      1.0f, 0.0f,         0.0f, 1.0f, 0.0f, // bottom right
         0.5f,  0.5f, -0.5f,    1.f, 0.f, 0.f,      1.0f, 1.0f,         0.0f, 1.0f, 0.0f, // top right

        //Bottom Quad       
        -0.5f, -0.5f,  0.5f,    0.f, 1.f, 1.f,      0.0f, 1.0f,         0.0f, -1.0f, 0.0f, // top left
        -0.5f, -0.5f, -0.5f,    0.f, 1.f, 1.f,      0.0f, 0.0f,         0.0f, -1.0f, 0.0f, // bottom left
         0.5f, -0.5f, -0.5f,    1.f, 1.f, 0.f,      1.0f, 0.0f,         0.0f, -1.0f, 0.0f, // bottom right
         0.5f, -0.5f,  0.5f,    1.f, 1.f, 0.f,      1.0f, 1.0f,         0.0f, -1.0f, 0.0f // top right
     };
    
    std::vector<uint32_t> indices = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };  

    uint32_t m_DrawMode = GL_TRIANGLES;
    std::shared_ptr<GLVertexArray> va;
    std::shared_ptr<GLIndexBuffer> ib;
    std::shared_ptr<GLVertexBuffer> vb;
    std::shared_ptr<Shader> shader;
    std::vector<Texture*> textures;
    std::vector<std::string> texturepaths;
    std::string meshpath;
};
