#include "Pyramid.hpp"
#include "BindableBase.hpp"
#include "GraphicsThrowMacros.hpp"
#include "Cone.hpp"

namespace dx = DirectX;

Pyramid::Pyramid(Graphics& gfx,
    std::mt19937& rng,
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
    if (!IsStaticInitialized())
    {
        struct Vertex
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

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices_));
        auto pvs = std::make_unique<VertexShader>(gfx, L"VSColorBlend.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"PSColorBlend.cso"));
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices_));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexFromStatic();
    }

    AddBind(std::make_unique<TransformCbuf>(gfx, *this));
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

DirectX::XMMATRIX Pyramid::GetTransformXM() const noexcept
{
    return dx::XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_) *
        dx::XMMatrixTranslation(r_, 0.0f, 0.0f) *
        dx::XMMatrixRotationRollPitchYaw(theta_, phi_, chi_) *
        dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}