#pragma once

#include "FatWin.hpp"

#include <d3d11.h>

class Graphics
{
public:
    Graphics(HWND hWnd);
    ~Graphics();
    Graphics(const Graphics& src) = delete;
    Graphics(Graphics&& src) = delete;
    Graphics& operator = (const Graphics& src) = delete;
    Graphics& operator = (Graphics&& src) = delete;


public:
    void EndFrame();


private:
    ID3D11Device* pDevice = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;
    ID3D11DeviceContext* pDeviceContext = nullptr;
};
