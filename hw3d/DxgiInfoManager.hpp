#pragma once

#include "FatWin.hpp"

#include <dxgidebug.h>
#include <wrl.h>

#include <vector>
#include <string>

class DxgiInfoManager
{
public:
    DxgiInfoManager();
    ~DxgiInfoManager() = default;
    DxgiInfoManager(const DxgiInfoManager&) = delete;
    DxgiInfoManager(DxgiInfoManager&&) = delete;
    DxgiInfoManager& operator = (const DxgiInfoManager&) = delete;
    DxgiInfoManager& operator = (DxgiInfoManager&&) = delete;


public:
    std::vector<std::string> GetMessages() const;

    void Set() noexcept;


private:
    Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue = nullptr;

    unsigned long long nextIndex = 0u;
};