#include "Sheet.hpp"

#include <Utility/include/Surface.hpp>

#include <Win32_/D3D11/Core/include/InputLayout.hpp>
#include <Win32_/D3D11/Core/include/Topology.hpp>
#include <Win32_/D3D11/Resource/include/Texture2D.hpp>
#include <Win32_/D3D11/Resource/include/TransformCBuffer.hpp>
#include <Win32_/D3D11/Resource/include/ShaderResource.hpp>
#include <Win32_/D3D11/Shader/include/PixelShader.hpp>
#include <Win32_/D3D11/Shader/include/Sampler.hpp>
#include <Win32_/D3D11/Shader/include/VertexShader.hpp>

#include "Base/Plane.hpp"

namespace dx = DirectX;

using namespace fatpound::win32::d3d11;

namespace hw3d::obj
{
    Sheet::Sheet(ID3D11Device* const pDevice,
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
        if (not DrawableBase::IsStaticInitialized_())
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

            auto model = base::Plane::Make<Vertex>();
            model.vertices_[0].tex = { 0.0f,0.0f };
            model.vertices_[1].tex = { 1.0f,0.0f };
            model.vertices_[2].tex = { 0.0f,1.0f };
            model.vertices_[3].tex = { 1.0f,1.0f };

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

            FATSPACE_D3D11::resource::Texture2D tex2d(pDevice, tex2dDesc, pSurface);

            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::resource::ShaderResource>(pDevice, tex2d, srvDesc));

            {
                const D3D11_SAMPLER_DESC sDesc
                {
                    .Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
                    .AddressU       = D3D11_TEXTURE_ADDRESS_WRAP,
                    .AddressV       = D3D11_TEXTURE_ADDRESS_WRAP,
                    .AddressW       = D3D11_TEXTURE_ADDRESS_WRAP,
                    .MipLODBias     = {},
                    .MaxAnisotropy  = {},
                    .ComparisonFunc = D3D11_COMPARISON_NEVER,
                    .BorderColor    = {},
                    .MinLOD         = 0.0F,
                    .MaxLOD         = D3D11_FLOAT32_MAX
                };

                DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::shader::Sampler>(pDevice, sDesc));
            }


            using VxBufVal_t = decltype(model.vertices_)::value_type;

            const D3D11_BUFFER_DESC vbd
            {
                .ByteWidth = static_cast<UINT>(sizeof(VxBufVal_t) * model.vertices_.size()),
                .Usage = D3D11_USAGE_DEFAULT,
                .BindFlags = D3D11_BIND_VERTEX_BUFFER,
                .CPUAccessFlags = 0U,
                .MiscFlags = 0U,
                .StructureByteStride = sizeof(VxBufVal_t)
            };

            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::resource::VertexBuffer>(pDevice, vbd, model.vertices_));

            Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob;

            if (FAILED(D3DReadFileToBlob(L"VSTexture.cso", &pVSBlob)))
            {
                throw std::runtime_error("CANNOT read shader file to D3D Blob!");
            }

            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::shader::VertexShader>(pDevice, pVSBlob));
            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::shader::PixelShader>(pDevice, std::wstring{ L"PSTexture.cso" }));

            using IdxBufVal_t = decltype(model.indices_)::value_type;

            D3D11_BUFFER_DESC ibd =
            {
                .ByteWidth = static_cast<UINT>(model.indices_.size() * sizeof(IdxBufVal_t)),
                .Usage = D3D11_USAGE_DEFAULT,
                .BindFlags = D3D11_BIND_INDEX_BUFFER,
                .CPUAccessFlags = 0u,
                .MiscFlags = 0u,
                .StructureByteStride = sizeof(IdxBufVal_t)
            };

            AddStaticIndexBuffer_(std::make_unique<FATSPACE_D3D11::resource::IndexBuffer>(pDevice, ibd, DXGI_FORMAT_R16_UINT, model.indices_));

            const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
            {
                { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };

            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::core::InputLayout>(pDevice, ied, pVSBlob));
            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::core::Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
        }
        else
        {
            SetIndexBufferFromStatic_();
        }

        const D3D11_BUFFER_DESC cbd
        {
            .ByteWidth = sizeof(DirectX::XMMATRIX),
            .Usage = D3D11_USAGE_DYNAMIC,
            .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
            .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
            .MiscFlags = 0U,
            .StructureByteStride = 0U
        };

        AddBind_(std::make_unique<FATSPACE_D3D11::resource::TransformCBuffer<Sheet>>(pDevice, cbd, *this, viewXM));
    }

    void Sheet::Update(float dt) noexcept
    {
        roll_  += droll_  * dt;
        pitch_ += dpitch_ * dt;
        yaw_   += dyaw_   * dt;
        theta_ += dtheta_ * dt;
        phi_   += dphi_   * dt;
        chi_   += dchi_   * dt;
    }

    auto Sheet::GetTransformXM() const noexcept -> dx::XMMATRIX
    {
        return dx::XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_) *
            dx::XMMatrixTranslation(r_, 0.0f, 0.0f) *
            dx::XMMatrixRotationRollPitchYaw(theta_, phi_, chi_);
    }
}