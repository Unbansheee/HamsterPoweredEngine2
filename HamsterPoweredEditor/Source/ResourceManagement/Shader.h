#pragma once
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct PairHash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    unsigned int rendererID;
    std::string vertexPath;
    std::string fragmentPath;
    std::unordered_map<std::string, int> uniformLocationCache;
    
public:
    ~Shader();


    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform2f(const std::string& name, float v0, float v1);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& value);
    
    int GetUniformLocation(const std::string& name);

    static std::shared_ptr<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);
    static void ReloadAllShaders();
    
    void ReloadShader();
    
private:
    unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);
    static ShaderProgramSource CreateShaderSource(const std::string& vertexPath, const std::string& fragmentPath);
    static std::string ReadFile(const std::string& path);
    static void PreprocessShader(std::string& source, std::vector<std::string>* includedPaths);
    Shader(const std::string& vertex, const std::string& fragment);
    
    inline static std::vector<std::string> includePaths;

    // Cache of shader programs
    inline static std::unordered_map<std::pair<std::string, std::string>, std::shared_ptr<Shader>, PairHash> m_shaderCache;

    ShaderProgramSource m_Source;
    
};

