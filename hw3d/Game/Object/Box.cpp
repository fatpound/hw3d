#include "Box.hpp"

#include "../../Win32_/DirectX/D3D11/Pipeline/BindableBase.hpp"

#include "../../Win32_/DirectX/D3D11/GraphicsThrowMacros.hpp"

#include "Base/Cube.hpp"

namespace dx = DirectX;

using namespace fatpound::win32::d3d11;

namespace fatpound::hw3d::obj
{
    Box::Box(Graphics& gfx,
        std::minstd_rand& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist,
        std::uniform_real_distribution<float>& bdist)
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
            };

            auto model = base::Cube::Make<Vertex>();

            DrawableBase::AddStaticBind_(std::make_unique<NAMESPACE_PIPELINE::VertexBuffer>(gfx, model.vertices_));

            auto pvs = std::make_unique<NAMESPACE_PIPELINE::VertexShader>(gfx, L"VSColorIndex.cso");
            auto pvsbc = pvs->GetBytecode();

            DrawableBase::AddStaticBind_(std::move(pvs));
            DrawableBase::AddStaticBind_(std::make_unique<NAMESPACE_PIPELINE::PixelShader>(gfx, L"PSColorIndex.cso"));

            AddStaticIndexBuffer_(std::make_unique<NAMESPACE_PIPELINE::IndexBuffer>(gfx, model.indices_));

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
                    { 1.0f, 1.0f, 1.0f },
                    { 1.0f, 0.0f, 0.0f },
                    { 0.0f, 1.0f, 0.0f },
                    { 1.0f, 1.0f, 0.0f },
                    { 0.0f, 0.0f, 1.0f },
                    { 1.0f, 0.0f, 1.0f },
                    { 0.0f, 1.0f, 1.0f },
                    { 0.0f, 0.0f, 0.0f }
                }
            };

            DrawableBase::AddStaticBind_(std::make_unique<NAMESPACE_PIPELINE::PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));

            const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
            {
                { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };

            DrawableBase::AddStaticBind_(std::make_unique<NAMESPACE_PIPELINE::InputLayout>(gfx, ied, pvsbc));
            DrawableBase::AddStaticBind_(std::make_unique<NAMESPACE_PIPELINE::Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
        }
        else
        {
            SetIndexFromStatic_();
        }

        AddBind_(std::make_unique<NAMESPACE_PIPELINE::TransformCBuf>(gfx, *this));

        // model deformation transform per instance
        dx::XMStoreFloat3x3(&mt_, dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
    }

    void Box::Update(float deltaTime) noexcept
    {
        roll_  += droll_  * deltaTime;
        pitch_ += dpitch_ * deltaTime;
        yaw_   += dyaw_   * deltaTime;
        theta_ += dtheta_ * deltaTime;
        phi_   += dphi_   * deltaTime;
        chi_   += dchi_   * deltaTime;
    }

    auto Box::GetTransformXM() const noexcept -> dx::XMMATRIX
    {
        return dx::XMLoadFloat3x3(&mt_) *
            dx::XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_) *
            dx::XMMatrixTranslation(r_, 0.0f, 0.0f) *
            dx::XMMatrixRotationRollPitchYaw(theta_, phi_, chi_);
    }
}