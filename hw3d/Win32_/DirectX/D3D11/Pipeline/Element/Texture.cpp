#include "Texture.hpp"

namespace wrl = Microsoft::WRL;

namespace fatpound::win32::d3d11::pipeline::element
{
    Texture::Texture(ID3D11Device* const pDevice, const FATSPACE_UTIL::Surface& surface)
    {
        D3D11_TEXTURE2D_DESC texDesc = {};
        texDesc.Width = surface.GetWidth<UINT>();
        texDesc.Height = surface.GetHeight<UINT>();
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
        sd.pSysMem = const_cast<FATSPACE_UTIL::Surface&>(surface);
        sd.SysMemPitch = surface.GetWidth<UINT>() * sizeof(FATSPACE_UTIL::Color);

        wrl::ComPtr<ID3D11Texture2D> pTexture;

        pDevice->CreateTexture2D(&texDesc, &sd, &pTexture);

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Format = texDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        pDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView_);
    }

    void Texture::Bind(ID3D11DeviceContext* const pImmediateContext) noexcept
    {
        pImmediateContext->PSSetShaderResources(0u, 1u, pTextureView_.GetAddressOf());
    }
}