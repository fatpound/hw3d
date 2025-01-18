#include "ImguiManager.hpp"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

namespace hw3d
{
    ImguiManager::ImguiManager(const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pImmediateContext)
    {
        IMGUI_CHECKVERSION();

        ::ImGui::CreateContext();
        ::ImGui::StyleColorsDark();
        ::ImGui_ImplWin32_Init(hWnd);
        ::ImGui_ImplDX11_Init(pDevice, pImmediateContext);
    }
    ImguiManager::~ImguiManager()
    {
        ::ImGui_ImplWin32_Shutdown();
        ::ImGui_ImplDX11_Shutdown();
        ::ImGui::DestroyContext();
    }
}