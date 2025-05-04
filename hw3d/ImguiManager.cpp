#include "ImguiManager.hpp"

#include <FatDefines.hpp>

#pragma warning (push)
#pragma warning (disable : FAT_EXTERNAL_WARNINGS)
#include <imgui.h>
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