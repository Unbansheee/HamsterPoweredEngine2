#pragma once
#include <assert.h>
#include <string>
#include <vector>
#include "glad/glad.h"

enum class ShaderDataType
{
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat4,
    Mat3,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
        return sizeof(float);
    case ShaderDataType::Float2:
        return sizeof(float) * 2;
    case ShaderDataType::Float3:
        return sizeof(float) * 3;
    case ShaderDataType::Float4:
        return sizeof(float) * 4;
    case ShaderDataType::Mat4:
        return sizeof(float) * 16;
    case ShaderDataType::Mat3:
        return sizeof(float) * 12;
    case ShaderDataType::Int:
        return sizeof(int);
    case ShaderDataType::Int2:
        return sizeof(int) * 2;
     case ShaderDataType::Int3:
        return sizeof(int) * 3;
    case ShaderDataType::Int4:
        return sizeof(int) * 4;
    case ShaderDataType::Bool:
        return sizeof(bool);
        
    }
    return 0;
}

static GLenum ShaderDataTypeToGL(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
        return GL_FLOAT;
    case ShaderDataType::Float2:
        return GL_FLOAT;
    case ShaderDataType::Float3:
        return GL_FLOAT;
    case ShaderDataType::Float4:
        return GL_FLOAT;
    case ShaderDataType::Mat4:
        return GL_FLOAT;
    case ShaderDataType::Mat3:
        return GL_FLOAT;
    case ShaderDataType::Int:
        return GL_INT;
    case ShaderDataType::Int2:
        return GL_INT;
    case ShaderDataType::Int3:
        return GL_INT;
    case ShaderDataType::Int4:
        return GL_INT;
    case ShaderDataType::Bool:
        return GL_BOOL;
    }
    return 0;
}

struct VertexBufferElement
{
    std::string Name;
    ShaderDataType Type;
    uint32_t Size;
    uint32_t Offset;
    bool Normalized;

    VertexBufferElement() = default;
    VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized = false) : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
    {
    }

    uint32_t GetComponentCount() const
    {
        switch (Type)
        {
            case ShaderDataType::Float: return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat4: return 4 * 4;
            case ShaderDataType::Mat3: return 3 * 3;
            case ShaderDataType::Int: return 1;
            case ShaderDataType::Int2: return 2;
            case ShaderDataType::Int3: return 3;
            case ShaderDataType::Int4: return 4;
            case ShaderDataType::Bool: return 1;
            case ShaderDataType::None:
            default: return 0;
        }
    }
};

class VertexBufferLayout
{
    std::vector<VertexBufferElement> m_Elements;
    uint32_t m_Stride = 0;
public:
    VertexBufferLayout() = default;
    VertexBufferLayout(std::initializer_list<VertexBufferElement> elements) : m_Elements(elements)
    {
        CalculateOffsetsAndStride();
    }

    std::vector<VertexBufferElement>::iterator begin() { return m_Elements.begin(); }
    std::vector<VertexBufferElement>::iterator end() { return m_Elements.end(); }
    std::vector<VertexBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
    std::vector<VertexBufferElement>::const_iterator end() const { return m_Elements.end(); }
    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    inline uint32_t GetStride() const { return m_Stride; }

private:
    void CalculateOffsetsAndStride()
    {
        uint32_t offset = 0;
        for (auto& element : m_Elements)
        {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
        
    }

};
