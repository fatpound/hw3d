#include "Texture.hpp"
#include "Surface.hpp"
#include "GraphicsThrowMacros.hpp"

namespace wrl = Microsoft::WRL;

Texture::Texture(Graphics& gfx, const Surface& surface)
{
    INFOMAN(gfx);

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = surface.GetWidth();
    texDesc.Height = surface.GetHeight();
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = surface.GetBufferPtr();
    sd.SysMemPitch = surface.GetWidth() * sizeof(Surface::Color);

    wrl::ComPtr<ID3D11Texture2D> pTexture;
    
    GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(
        &texDesc, &sd, &pTexture
    ));

    // creating the resource view on the texture
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
        pTexture.Get(), &srvDesc, &pTextureView_
    ));
}

void Texture::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView_.GetAddressOf());
}
