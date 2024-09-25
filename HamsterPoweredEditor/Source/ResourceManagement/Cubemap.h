#pragma once
#include <cstdint>
#include <string>

class Cubemap
{
public:
    Cubemap(const std::string& path);
    ~Cubemap();

    void Bind(int slot = 0);
    void Unbind();
    uint32_t ID() const { return id; }
    
    uint32_t id = 0;
    std::string path;
    
};
