#pragma once

#include "../../../FatWin32_.hpp"

#include <dxgidebug.h>
#include <wrl.h>

#include <vector>
#include <string>

class DxgiInfoManager final
{
public:
    DxgiInfoManager();
    DxgiInfoManager(const DxgiInfoManager&) = delete;
    DxgiInfoManager& operator = (const DxgiInfoManager&) = delete;
    DxgiInfoManager(DxgiInfoManager&&) = delete;
    DxgiInfoManager& operator = (DxgiInfoManager&&) = delete;
    ~DxgiInfoManager() = default;


public:
    auto GetMessages() const -> std::vector<std::string>;

    void SetNextIndex() noexcept;


protected:


private:
    Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue_ = nullptr;

    unsigned long long next_error_index_ = 0u;
};