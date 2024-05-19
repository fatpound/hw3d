#include "ImguiManager.hpp"

#include "../imgui/imgui.h"

namespace fatpound::util
{
    ImguiManager::ImguiManager()
    {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
    }

    ImguiManager::~ImguiManager()
    {
        ImGui::DestroyContext();
    }
}