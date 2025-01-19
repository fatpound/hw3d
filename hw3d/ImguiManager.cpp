#include "ImguiManager.hpp"

#include <imgui.h>

namespace hw3d
{
    ImguiManager::ImguiManager()
    {
        IMGUI_CHECKVERSION();

        ::ImGui::CreateContext();
        ::ImGui::StyleColorsDark();
    }
    ImguiManager::~ImguiManager()
    {
        ::ImGui::DestroyContext();
    }
}