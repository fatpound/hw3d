#pragma once

#include "../../FatWin32_.hpp"

#include <dxgidebug.h>
#include <wrl.h>

#include <vector>
#include <string>

namespace fatpound::win32::dxgi
{
    class InfoManager final
    {
    public:
        InfoManager();
        InfoManager(const InfoManager&) = delete;
        InfoManager& operator = (const InfoManager&) = delete;

        InfoManager(InfoManager&&) = delete;
        InfoManager& operator = (InfoManager&&) = delete;
        ~InfoManager() noexcept = default;


    public:
        auto GetMessages() const -> std::vector<std::string>;

        void SetNextIndex() noexcept;


    protected:


    private:
        Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue_ = nullptr;

        unsigned long long int next_error_index_ = 0u;
    };
}