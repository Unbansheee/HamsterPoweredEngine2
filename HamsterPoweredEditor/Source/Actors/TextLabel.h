#pragma once
#include <map>

#include "Rendering/GLVertexArray.h"
#include "Actors/Actor.h"
#include "Core/Font.h"
#include "ResourceManagement/Shader.h"

struct Bounds
{
    float x;
    float y;
    float width;
    float height;
};

class TextLabel : public Actor
{
public:
    TextLabel(const std::string& text = "Text Label", const std::string& fontPath = "Resources/Fonts/Roboto-Regular.ttf", int fontSize = 100, const glm::vec3& color = {0, 1, 1}, const glm::vec3& position = {0, 0, 0});
    void SetText(const std::string& text);
    void SetCurrentColor(const glm::vec3& color);
    void SetBaseColor(const glm::vec3& color);
    void SetHoverColor(const glm::vec3& color);
    void SetClickColor(const glm::vec3& color);
    void SetFont(Font* font);
    Font* GetFont() const;

    

    void Begin() override;
    void OnDestroy() override;
    void Update(Timestep ts) override;
    void Draw() override;
protected:
    void OnInspectorGUI() override;
    virtual void UpdateBuffers();
public:
    ~TextLabel() override;
    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;
    void FixedUpdate(double ts) override;

protected:
    
    
    std::string m_Text = "Text Label";
    std::string m_FontName;
    int m_FontSize;
    glm::vec3 m_Color;
    glm::vec3 m_HoverColor = {0, 1, 0};
    glm::vec3 m_PressedColor = {0, 0, 1};
    glm::vec3 m_BaseColor = {1, 1, 1};
    bool mouseOverlapping = false;
    std::shared_ptr<GLVertexArray> m_VAO;
    std::shared_ptr<GLVertexBuffer> m_VBO;
    std::shared_ptr<GLIndexBuffer> m_IBO;
    std::shared_ptr<Shader> m_Shader;
    Font* m_Font;
    std::string m_FontPath;
    glm::mat4 m_Projection;
    glm::mat4 m_View;
    bool m_ScreenSpace = true;
    glm::vec2 mouseoffset = {0, 0};
    bool m_Dragging = false;
    bool m_Initialized = false;
    Bounds m_bounds;
    Bounds m_unscaledBounds;
    std::vector<Texture*> textures;

    bool m_scaleBounce = false;
    float m_scaleBounceDelta = 0.0f;
    bool m_sideScroll = false;
    glm::vec2 m_scrollSpeed = {0, 0};
    
};
