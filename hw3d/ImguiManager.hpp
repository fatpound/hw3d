#pragma once

#include <FatWin32.hpp>

#include <d3d11.h>

namespace hw3d
{
    class ImguiManager final
    {
    public:
        ImguiManager(const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pImmediateContext);

        ImguiManager() = delete;
        ImguiManager(const ImguiManager& src) = delete;
        ImguiManager(ImguiManager&& src) = delete;

        auto operator = (const ImguiManager& src) -> ImguiManager& = delete;
        auto operator = (ImguiManager&& src)      -> ImguiManager& = delete;
        ~ImguiManager();


    protected:


    private:
    };
}