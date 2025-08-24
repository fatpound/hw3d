#include "Melon.hpp"

#include "Base/Sphere.hpp"

#include <Win32_/D3D11/Core/include/InputLayout.hpp>
#include <Win32_/D3D11/Core/include/Topology.hpp>
#include <Win32_/D3D11/Resource/include/PixelCBuffer.hpp>
#include <Win32_/D3D11/Resource/include/TransformCBuffer.hpp>
#include <Win32_/D3D11/Shader/include/PixelShader.hpp>
#include <Win32_/D3D11/Shader/include/VertexShader.hpp>

namespace dx = DirectX;

namespace hw3d::obj
{
    Melon::Melon(ID3D11Device* const pDevice,
        std::minstd_rand& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist,
        std::uniform_int_distribution<int>& longdist,
        std::uniform_int_distribution<int>& latdist,
        FATSPACE_UTILITY::ViewXM& viewXM)
        :
        r_(rdist(rng)),
        theta_(adist(rng)),
        phi_(adist(rng)),
        chi_(adist(rng)),
        droll_(ddist(rng)),
        dpitch_(ddist(rng)),
        dyaw_(ddist(rng)),
        dtheta_(odist(rng)),
        dphi_(odist(rng)),
        dchi_(odist(rng))
    {
        if (not DrawableBase::IsStaticInitialized_())
        {
            Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob;

            if (FAILED(D3DReadFileToBlob(L"VSColorIndex.cso", &pVSBlob)))
            {
                throw std::runtime_error("CANNOT read shader file to D3D Blob!");
            }

            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::shader::VertexShader>(pDevice, pVSBlob));

            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::shader::PixelShader>(pDevice, std::wstring{ L"PSColorIndex.cso" }));

            struct PixelShaderConstants final
            {
                struct
                {
                    float r;
                    float g;
                    float b;
                    float a;
                }
                face_colors[8];
            };

            const PixelShaderConstants cb2 =
            {
                {
                    { 1.0f,1.0f,1.0f },
                    { 1.0f,0.0f,0.0f },
                    { 0.0f,1.0f,0.0f },
                    { 1.0f,1.0f,0.0f },
                    { 0.0f,0.0f,1.0f },
                    { 1.0f,0.0f,1.0f },
                    { 0.0f,1.0f,1.0f },
                    { 0.0f,0.0f,0.0f },
                }
            };

            const D3D11_BUFFER_DESC pcbd
            {
                .ByteWidth = sizeof(PixelShaderConstants),
                .Usage = D3D11_USAGE_DYNAMIC,
                .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
                .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
                .MiscFlags = 0U,
                .StructureByteStride = 0U
            };

            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::resource::PixelCBuffer<PixelShaderConstants>>(pDevice, pcbd, cb2));

            const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
            {
                { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
            };

            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::core::InputLayout>(pDevice, ied, pVSBlob));
            DrawableBase::AddStaticBind_(std::make_unique<FATSPACE_D3D11::core::Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
        }

        struct Vertex final
        {
            dx::XMFLOAT3 pos;
        };

        auto model = base::Sphere::MakeTesselated<Vertex>(latdist(rng), longdist(rng));

        model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 1.2f));

        using VxBufVal_t = decltype(model.vertices_)::value_type;

        const D3D11_BUFFER_DESC vbd
        {
            .ByteWidth           = static_cast<UINT>(sizeof(VxBufVal_t) * model.vertices_.size()),
            .Usage               = D3D11_USAGE_DEFAULT,
            .BindFlags           = D3D11_BIND_VERTEX_BUFFER,
            .CPUAccessFlags      = 0U,
            .MiscFlags           = 0U,
            .StructureByteStride = sizeof(VxBufVal_t)
        };


        AddBind_(std::make_unique<FATSPACE_D3D11::resource::VertexBuffer>(pDevice, vbd, model.vertices_));

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

        AddIndexBuffer_(std::make_unique<FATSPACE_D3D11::resource::IndexBuffer> (pDevice, ibd, DXGI_FORMAT_R16_UINT, model.indices_));


        const D3D11_BUFFER_DESC cbd
        {
            .ByteWidth = sizeof(DirectX::XMMATRIX),
            .Usage = D3D11_USAGE_DYNAMIC,
            .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
            .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
            .MiscFlags = 0U,
            .StructureByteStride = 0U
        };


        AddBind_(std::make_unique<FATSPACE_D3D11::resource::TransformCBuffer<Melon>>(pDevice, cbd, *this, viewXM));
    }

    void Melon::Update(const float deltaTime) noexcept
    {
        roll_  += droll_  * deltaTime;
        pitch_ += dpitch_ * deltaTime;
        yaw_   += dyaw_   * deltaTime;
        theta_ += dtheta_ * deltaTime;
        phi_   += dphi_   * deltaTime;
        chi_   += dchi_   * deltaTime;
    }

    auto Melon::GetTransformXM() const noexcept -> dx::XMMATRIX
    {
        return dx::XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_)
             * dx::XMMatrixTranslation(r_, 0.0f, 0.0f)
             * dx::XMMatrixRotationRollPitchYaw(theta_, phi_, chi_);
    }
}