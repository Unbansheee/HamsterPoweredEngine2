#include "ThemeUI.h"
#include "ThemeManager.h"

void ThemeUI::Update(Timestep ts)
{
	if (ImGui::BeginCombo("Theme", ThemeManager::currentTheme.c_str()))
	{
		for (auto i : ThemeManager::themes)
		{
			if (ImGui::MenuItem(i.first.c_str(), nullptr, nullptr))
			{
				ThemeManager::LoadTheme(i.second, i.first);
			}
		}
		ImGui::EndCombo();
	}

}

void ThemeUI::Init()
{
		name = "Theme Manager";
		canBeClosed = true;
		
}
