#pragma once

#include <Win32_/include/FatWin.hpp>

#include <d3d11.h>

namespace hw3d
{
    class ImguiManager final
    {
    public:
        ImguiManager();
        ImguiManager(const ImguiManager&)     = delete;
        ImguiManager(ImguiManager&&) noexcept = delete;

        auto operator = (const ImguiManager&)     -> ImguiManager& = delete;
        auto operator = (ImguiManager&&) noexcept -> ImguiManager& = delete;
        ~ImguiManager() noexcept(false);


    protected:


    private:
    };
}