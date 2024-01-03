#include "Graphics.hpp"

#include "dxerr.h"

#include <d3dcompiler.h>

#include <array>
#include <sstream>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "D3DCompiler")

#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__, __FILE__, (hr) )
#define GFX_THROW_NOINFO(hrcall) if (FAILED( hr = (hrcall) )) throw Graphics::HrException( __LINE__, __FILE__, (hr) )

#ifndef NDEBUG

#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__, __FILE__, (hr), infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.SetNextIndex(); if (FAILED( hr = (hrcall) )) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__, __FILE__, (hr), infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.SetNextIndex(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#else

#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__, __FILE__, (hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__, __FILE__, (hr) )
#define GFX_THROW_INFO_ONLY(call) (call)

#endif

namespace wrl = Microsoft::WRL;

// Graphics

Graphics::Graphics(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC scd = {};

    scd.BufferDesc.Width = 0; // these two zeroes mean go find out yourself from the hWnd
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

    UINT swapCreateFlags = 0u;

#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr;

    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        swapCreateFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scd,
        &pSwapChain,
        &pDevice,
        nullptr,
        &pContext)
    );

    wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
    
    GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));

    GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
}


void Graphics::EndFrame()
{
    HRESULT hr;

#ifndef NDEBUG
    infoManager.SetNextIndex();
#endif

    if (FAILED(hr = pSwapChain->Present(1u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else
        {
            throw GFX_EXCEPT(hr);
        }
    }
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
    const std::array<float, 4> colors = { red, green, blue, 1.0f };

    pContext->ClearRenderTargetView(pTarget.Get(), colors.data());
}

void Graphics::DrawTestTriangle(float angle)
{
    struct Vertex
    {
        struct
        {
            float x;
            float y;
        }
        pos;

        struct
        {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
        }
        color;
    };


    // create vertex buffer
    std::array<Vertex, 6> vertices =
    {
        Vertex{  0.0f,  0.5f, 255,   0,   0,   0 },
        Vertex{  0.5f, -0.5f,   0, 255,   0,   0 },
        Vertex{ -0.5f, -0.5f,   0,   0, 255,   0 },

        Vertex{ -0.3f,  0.3f,   0, 255,   0,   0 },
        Vertex{  0.3f,  0.3f,   0,   0, 255,   0 },
        Vertex{  0.0f, -1.0f, 255,   0,   0,   0 },
    };

    vertices[0].color.g = 255;

    D3D11_BUFFER_DESC bd = {};

    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices.data();

    HRESULT hr;

    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;

    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));


    // bind vertex buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;

    pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


    // create index buffer
    const std::array<unsigned short int, 12> indices =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 1,
        2, 1, 5
    };

    wrl::ComPtr<ID3D11Buffer> pIndexBuffer;

    D3D11_BUFFER_DESC ibd = {};

    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = sizeof(indices);
    ibd.StructureByteStride = sizeof(unsigned short);

    D3D11_SUBRESOURCE_DATA isd = {};

    isd.pSysMem = indices.data();

    GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));


    // bind index buffer
    pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


    // create constant buffer
    struct ConstantBuffer
    {
        struct
        {
            float element[4][4];
        }
        transformation;
    };

    const ConstantBuffer cb =
    {
        {
            std::cos(angle),    std::sin(angle),    0.0f,    0.0f,
            -std::sin(angle),   std::cos(angle),    0.0f,    0.0f,
            0.0f,               0.0f,               1.0f,    0.0f,
            0.0f,               0.0f,               0.0f,    1.0f
        }
    };

    wrl::ComPtr<ID3D11Buffer> pConstantBuffer;

    D3D11_BUFFER_DESC cbd = {};

    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = sizeof(cb);
    cbd.StructureByteStride = 0u;

    D3D11_SUBRESOURCE_DATA csd = {};

    csd.pSysMem = &cb;

    GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));


    // bind constant buffer to vertex shader
    pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());


    // create pixel shader
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    wrl::ComPtr<ID3DBlob> pBlob;

    GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));


    // bind pixel shader
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


    // create vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;

    GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));


    // bind vertex shader
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


    // input layout for vertices (2d position only)
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;

    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "Position", 0, DXGI_FORMAT_R32G32_FLOAT,   0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "Color",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    
    GFX_THROW_INFO(pDevice->CreateInputLayout(
        ied,
        static_cast<UINT>(std::size(ied)),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ));


    // bind vertex layout
    pContext->IASetInputLayout(pInputLayout.Get());


    // bind render target
    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);


    // set primitive topology to triangle list
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    // configure viewport
    D3D11_VIEWPORT vp = {};

    vp.Width = 800.0f;
    vp.Height = 600.0f;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;

    pContext->RSSetViewports(1u, &vp);

    GFX_THROW_INFO_ONLY(pContext->DrawIndexed(std::size(indices), 0u, 0u));
}


// HrException

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file),
    hresult(hr)
{
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }

    if ( ! info.empty() )
    {
        info.pop_back(); // '\n'
    }
}


HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
    return hresult;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
    return DXGetErrorString(hresult);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
    std::array<char, 512> buffer;

    DXGetErrorDescription(hresult, buffer.data(), sizeof(buffer));

    return buffer.data();
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
    return info;
}

const char* Graphics::HrException::what() const noexcept
{
    std::ostringstream oss;

    oss << GetType() << '\n'
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << static_cast<unsigned long>(GetErrorCode()) << ")" << '\n'
        << "[Error String] " << GetErrorString() << '\n'
        << "[Description] " << GetErrorDescription() << std::endl;

    if ( ! info.empty() )
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << '\n' << std::endl;
    }

    oss << GetOriginString();

    whatBuffer = oss.str();

    return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
    return "Fat Graphics Exception";
}


// InfoException

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file)
{
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }

    if ( ! info.empty() )
    {
        info.pop_back();
    }
}


const char* Graphics::InfoException::what() const noexcept
{
    std::ostringstream oss;

    oss << GetType() << std::endl
        << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;

    oss << GetOriginString();

    whatBuffer = oss.str();

    return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
    return "Fat Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
    return info;
}


// DeviceRemovedException

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
    return "Fat Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}