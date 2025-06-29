#include "SkinnedBox.hpp"

#include "Base/Cube.hpp"

#include <string>

namespace dx = DirectX;

using namespace fatpound::win32::d3d11;

namespace hw3d::obj
{
    SkinnedBox::SkinnedBox(ID3D11Device* const pDevice,
        std::minstd_rand& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist,
        FATSPACE_UTILITY::ViewXM& viewXM)
        :
        r_(rdist(rng)),
        droll_(ddist(rng)),
        dpitch_(ddist(rng)),
        dyaw_(ddist(rng)),
        dphi_(odist(rng)),
        dtheta_(odist(rng)),
        dchi_(odist(rng)),
        chi_(adist(rng)),
        theta_(adist(rng)),
        phi_(adist(rng))
    {
        if (!DrawableBase::IsStaticInitialized_())
        {
            struct Vertex final
            {
                dx::XMFLOAT3 pos;

                struct
                {
                    float u;
                    float v;
                }
                tex;
            };

            const auto model = base::Cube::MakeSkinned<Vertex>();

            {
                auto pSurface = std::make_shared<FATSPACE_UTILITY::Surface>(std::wstring(L"Resource\\Image\\cube.png"));

                const D3D11_TEXTURE2D_DESC tex2dDesc
                {
                    .Width      = pSurface->GetWidth<UINT>(),
                    .Height     = pSurface->GetHeight<UINT>(),
                    .MipLevels  = 1U,
                    .ArraySize  = 1U,
                    .Format     = DXGI_FORMAT_B8G8R8A8_UNORM,
                    .SampleDesc =
                                {
                                    .Count   = 1,
                                    .Quality = 0
                                },
                    .Usage      = D3D11_USAGE_DEFAULT,
                    .BindFlags  = D3D11_BIND_SHADER_RESOURCE
                };

                const D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc
                {
                    .Format        = tex2dDesc.Format,
                    .ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
                    .Texture2D     =
                                   {
                                       .MostDetailedMip = {},
                                       .MipLevels       = tex2dDesc.MipLevels
                                   }
                };

                DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::pipeline::Texture2D>(pDevice, tex2dDesc, srvDesc, pSurface));
            }

            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::pipeline::VertexBuffer>(pDevice, model.vertices_));

            auto pvs = std::make_unique<FATSPACE_D3D11::pipeline::VertexShader>(pDevice, L"VSTexture.cso");
            auto pvsbc = pvs->GetBytecode();

            DrawableBase::AddStaticBind_(std::move(pvs));
            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::pipeline::PixelShader>(pDevice, L"PSTexture.cso"));

            AddStaticIndexBuffer_(std::make_unique<FATSPACE_D3D11::pipeline::IndexBuffer>(pDevice, model.indices_));

            const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
            {
                { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
                { "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
            };

            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::pipeline::InputLayout>(pDevice, ied, pvsbc));
            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::pipeline::Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
        }
        else
        {
            SetIndexBufferFromStatic_();
        }

        AddBind_(std::make_unique<FATSPACE_D3D11::pipeline::TransformCBuffer<SkinnedBox>>(pDevice, *this, viewXM));
    }

    void SkinnedBox::Update(float dt) noexcept
    {
        roll_  += droll_  * dt;
        pitch_ += dpitch_ * dt;
        yaw_   += dyaw_   * dt;
        theta_ += dtheta_ * dt;
        phi_   += dphi_   * dt;
        chi_   += dchi_   * dt;
    }

    auto SkinnedBox::GetTransformXM() const noexcept -> dx::XMMATRIX
    {
        return dx::XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_) *
            dx::XMMatrixTranslation(r_, 0.0f, 0.0f) *
            dx::XMMatrixRotationRollPitchYaw(theta_, phi_, chi_);
    }
}