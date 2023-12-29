#pragma once

#include "FatWin.hpp"

#include <vector>
#include <string>

class DxgiInfoManager
{
public:
    DxgiInfoManager();
    ~DxgiInfoManager();
    DxgiInfoManager(const DxgiInfoManager&) = delete;
    DxgiInfoManager(DxgiInfoManager&&) = delete;
    DxgiInfoManager& operator = (const DxgiInfoManager&) = delete;
    DxgiInfoManager& operator = (DxgiInfoManager&&) = delete;


public:
    std::vector<std::string> GetMessages() const;

    void Set() noexcept;


private:
    struct IDXGIInfoQueue* pDxgiInfoQueue = nullptr;

    unsigned long long nextIndex = 0u;
};