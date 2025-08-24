#include "ImguiManager.hpp"

#include <_misc/FatCodex/Macros/ExternalWarnings_MSVC.hpp>

#pragma warning (push)
#pragma warning (disable : MSVC_EXWARN_IMGUI)
#include <imgui/imgui.h>
#pragma warning (pop)

namespace hw3d
{
    ImguiManager::ImguiManager()
    {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
    }
    ImguiManager::~ImguiManager() noexcept(false)
    {
        ImGui::DestroyContext();
    }
}