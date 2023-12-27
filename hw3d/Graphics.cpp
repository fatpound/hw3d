#include "Graphics.hpp"

#pragma comment(lib, "d3d11")

Graphics::Graphics(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC scd = {};

    scd.BufferDesc.Width = 0; // these two means go find out yourself from the hWnd
    scd.BufferDesc.Height = 0;
    scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 0;
    scd.BufferDesc.RefreshRate.Denominator = 0;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1; // this is back buffer count, there will be a front buffer anyways
    scd.OutputWindow = hWnd;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = 0;

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scd,
        &pSwapChain,
        &pDevice,
        nullptr,
        &pDeviceContext
    );
}

Graphics::~Graphics()
{
    if (pDevice != nullptr)
    {
        pDevice->Release();
    }

    if (pSwapChain != nullptr)
    {
        pSwapChain->Release();
    }

    if (pDeviceContext != nullptr)
    {
        pDeviceContext->Release();
    }
}

void Graphics::EndFrame()
{
    pSwapChain->Present(1u, 0u);
}
