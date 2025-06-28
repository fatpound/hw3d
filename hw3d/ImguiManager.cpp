#include "ImguiManager.hpp"

#include <_macros/Compiler.hpp>

#pragma warning (push)
#pragma warning (disable : FATLIB_EXTERNAL_WARNINGS)
#include <imgui/imgui.h>
#pragma warning (pop)

namespace hw3d
{
    ImguiManager::ImguiManager()
    {
        IMGUI_CHECKVERSION();

        ::ImGui::CreateContext();
        ::ImGui::StyleColorsDark();
    }
    ImguiManager::~ImguiManager() noexcept(false)
    {
        ::ImGui::DestroyContext();
    }
}