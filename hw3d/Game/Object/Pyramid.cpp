#include "Pyramid.hpp"

#include "../../Win32_/DirectX/Direct3D11/Bindable/BindableBase.hpp"

#include "../../Win32_/DirectX/Direct3D11/Macro/GraphicsThrowMacros.hpp"

#include "Base/Cone.hpp"

namespace dx = DirectX;

Pyramid::Pyramid(Graphics& gfx,
    std::minstd_rand& rng,
    std::uniform_real_distribution<float>& adist,
    std::uniform_real_distribution<float>& ddist,
    std::uniform_real_distribution<float>& odist,
    std::uniform_real_distribution<float>& rdist)
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
                unsigned char r;
                unsigned char g;
                unsigned char b;
                unsigned char a;
            }
            color;
        };

        auto model = Cone::MakeTesselated<Vertex>(4);

        // set vertex colors for mesh
        model.vertices_[0].color = { 255,255,0 };
        model.vertices_[1].color = { 255,255,0 };
        model.vertices_[2].color = { 255,255,0 };
        model.vertices_[3].color = { 255,255,0 };
        model.vertices_[4].color = { 255,255,80 };
        model.vertices_[5].color = { 255,10,0 };

        // deform mesh linearly
        model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 0.7f));

        DrawableBase::AddStaticBind_(std::make_unique<VertexBuffer>(gfx, model.vertices_));

        auto pvs = std::make_unique<VertexShader>(gfx, L"VSColorBlend.cso");
        auto pvsbc = pvs->GetBytecode();

        DrawableBase::AddStaticBind_(std::move(pvs));
        DrawableBase::AddStaticBind_(std::make_unique<PixelShader>(gfx, L"PSColorBlend.cso"));
        AddStaticIndexBuffer_(std::make_unique<IndexBuffer>(gfx, model.indices_));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "Color",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        DrawableBase::AddStaticBind_(std::make_unique<InputLayout>(gfx, ied, pvsbc));
        DrawableBase::AddStaticBind_(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexFromStatic_();
    }

    AddBind_(std::make_unique<TransformCBuf>(gfx, *this));
}

void Pyramid::Update(float deltaTime) noexcept
{
    roll_  += droll_  * deltaTime;
    pitch_ += dpitch_ * deltaTime;
    yaw_   += dyaw_   * deltaTime;
    theta_ += dtheta_ * deltaTime;
    phi_   += dphi_   * deltaTime;
    chi_   += dchi_   * deltaTime;
}

auto Pyramid::GetTransformXM() const noexcept -> dx::XMMATRIX
{
    return dx::XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_) *
        dx::XMMatrixTranslation(r_, 0.0f, 0.0f) *
        dx::XMMatrixRotationRollPitchYaw(theta_, phi_, chi_);
}