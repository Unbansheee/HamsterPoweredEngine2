#pragma once

#include "imgui.h"
#include "Core/Font.h"
#include "ResourceManagement/Texture.h"
#include "imgui/imgui_stdlib.h"

#include "ImFileDialog.h"


class Font;

namespace ImGui
{
    inline void Image(std::shared_ptr<Texture> texture, const ImVec2& size, const ImVec4& tint_col = {1, 1, 1, 1}, const ImVec4& border_col = {0, 0, 0, 0})
    {
        Image((ImTextureID)(uint64_t)texture->rendererID, size, ImVec2{0, 1}, ImVec2{1, 0}, tint_col, border_col);
    }

    inline void Image(Texture* texture, const ImVec2& size, const ImVec4& tint_col = {1, 1, 1, 1}, const ImVec4& border_col = {0, 0, 0, 0})
    {
        
        Image((ImTextureID)(uint64_t)texture->rendererID, size, ImVec2{0, 1}, ImVec2{1, 0}, tint_col, border_col);  // NOLINT(clang-diagnostic-int-to-void-pointer-cast)
    }

    inline void ImageScaledH(Texture* texture, float scaleFactor = 1.f, bool clampSize = false, const ImVec4& tint_col = {1, 1, 1, 1}, const ImVec4& border_col = {0, 0, 0, 0})
    {
        ImVec2 size = {ImGui::GetContentRegionAvail().x*scaleFactor, ImGui::GetContentRegionAvail().x*scaleFactor / (float)texture->GetWidth() * (float)texture->GetHeight()};

        if (clampSize)
        {
            size.x = (std::min)(size.x, (float)texture->GetWidth());
            size.y = (std::min)(size.y, (float)texture->GetHeight());
        }

        
        Image((void*)(uint64_t)texture->rendererID, size, ImVec2{0, 1}, ImVec2{1, 0}, tint_col, border_col);
    }

    inline bool FontSelector(const char* label, Font** font)
    {
        bool edited = false;
        if (ImGui::BeginCombo("Font", (*font)->GetName().c_str()))
        {
            for (auto it = Font::fonts.begin(); it != Font::fonts.end(); it++)
            {
                bool is_selected = (it->first.first == (*font)->GetPath());
                if (ImGui::Selectable(it->second->GetName().c_str(), is_selected))
                {
                    (*font) = it->second;
                    edited = true;
                }
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        return edited;
    }

    inline bool TextureFilteringSelector(const char* label, Texture* texture)
    {
        const char* items[] = { "Nearest", "Linear" };
        int item_current = texture->GetFilteringMode();
        if (ImGui::Combo("Filtering Mode", &item_current, items, IM_ARRAYSIZE(items)))
        {
            if (item_current == 0)
            {
                texture->SetFilteringMode(Texture::FilteringMode::NEAREST);
                return true;
            }
            else if (item_current == 1)
            {
                texture->SetFilteringMode(Texture::FilteringMode::LINEAR);
                return true;
            }
        }
        return false;
    }
    
    
    inline bool FileDialogButton(const char* label, std::string& target, const std::string& title, const std::string& filter = ".*", const std::string& defaultPath = "Resources")
    {
        if (ImGui::Button(label))
        {
            ifd::FileDialog::Instance().Open(title, title, filter, false, defaultPath);
        }

        bool success = false;
        
        if (ifd::FileDialog::Instance().IsDone(title))
        {
            if (ifd::FileDialog::Instance().HasResult()) {
                target = ifd::FileDialog::Instance().GetResult().u8string();
                std::replace(target.begin(), target.end(), '\\', '/');
                
                success = true;
            }
            ifd::FileDialog::Instance().Close();
        }
        return success;
    }

    inline bool OpenFilePath(const char* label, std::string& target, const std::string& title = "Open File", const std::string& filter = ".*", const std::string& defaultPath = "Resources")
    {
        bool edited = false;
        if (FileDialogButton(("...##" + title).c_str(), target, title, filter, defaultPath))
        {
            edited = true;
        }
        ImGui::SameLine();
        if (ImGui::InputText((std::string(label) + "##" + title).c_str(), &target, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            edited = true;
        }
        return edited;
    }
    
}
