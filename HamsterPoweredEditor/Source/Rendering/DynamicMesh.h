#pragma once
#include <vector>

#include "StaticMesh.h"

#include "Rendering/GLVertexArray.h"
#include "ResourceManagement/Shader.h"


class DynamicMesh : public StaticMesh
{
public:
    ~DynamicMesh();
    
    void Update(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

private:
    
};
