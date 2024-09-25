#pragma once
#include "Quad.h"

class AnimatedQuad : public Quad
{
protected:
    void OnInspectorGUI() override;
public:
    AnimatedQuad();
    ~AnimatedQuad() override;
    void Draw() override;
    void Update(Timestep ts) override;
    void Begin() override;
    void OnDestroy() override;
    
    
    
    void SetFPS(float fps);
    Texture* SetSpriteSheet(std::string path, int frameCount, int rows, int columns);
    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;

private:
    float m_fps = 30.f;
    int m_rows = 1;
    int m_columns = 1;
    int m_spriteWidth = 1;
    int m_spriteHeight = 1;
    int m_currentFrame = 0;
    float m_animationTime = 0.f;
    int m_frameCount = 1;
    std::string m_Path;
};
