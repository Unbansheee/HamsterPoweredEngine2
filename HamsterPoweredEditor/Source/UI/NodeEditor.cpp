#include "NodeEditor.h"
#include "imnodes.h"

NodeEditor::~NodeEditor()
{
}

void NodeEditor::Init()
{
    UIComponent::Init();
    name = "Node Editor";
}

void NodeEditor::Begin()
{
    UIComponent::Begin();
    ImNodes::BeginNodeEditor();
}

void NodeEditor::Update(Timestep ts)
{
    UIComponent::Update(ts);
    
    
    ImNodes::BeginNode(1);
        ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("output node");
        ImNodes::EndNodeTitleBar();
        const int output_attr_id = 2;
        ImNodes::BeginOutputAttribute(output_attr_id);
            ImGui::Text("output pin");
        ImNodes::EndOutputAttribute();
    ImNodes::EndNode();

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    

}

void NodeEditor::End()
{
    ImNodes::EndNodeEditor();
    UIComponent::End();
}
