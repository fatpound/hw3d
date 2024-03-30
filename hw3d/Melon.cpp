#include "Melon.hpp"
#include "BindableBase.hpp"
#include "GraphicsThrowMacros.hpp"
#include "Sphere.hpp"

namespace dx = DirectX;

Melon::Melon(Graphics& gfx,
    std::mt19937& rng,
    std::uniform_real_distribution<float>& adist,
    std::uniform_real_distribution<float>& ddist,
    std::uniform_real_distribution<float>& odist,
    std::uniform_real_distribution<float>& rdist,
    std::uniform_int_distribution<int>& longdist,
    std::uniform_int_distribution<int>& latdist)
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
        auto pvs = std::make_unique<VertexShader>(gfx, L"VSColorIndex.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind_(std::move(pvs));
        AddStaticBind_(std::make_unique<PixelShader>(gfx, L"PSColorIndex.cso"));

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

        AddStaticBind_(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));
        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
        };
        AddStaticBind_(std::make_unique<InputLayout>(gfx, ied, pvsbc));
        AddStaticBind_(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }

    struct Vertex final
    {
        dx::XMFLOAT3 pos;
    };

    auto model = Sphere::MakeTesselated<Vertex>(latdist(rng), longdist(rng));

    model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 1.2f)); // deforming a little bit

    AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices_));
    AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices_));
    AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Melon::Update(float deltaTime) noexcept
{
    roll_  += droll_  * deltaTime;
    pitch_ += dpitch_ * deltaTime;
    yaw_   += dyaw_   * deltaTime;
    theta_ += dtheta_ * deltaTime;
    phi_   += dphi_   * deltaTime;
    chi_   += dchi_   * deltaTime;
}

DirectX::XMMATRIX Melon::GetTransformXM() const noexcept
{
    return dx::XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_) *
        dx::XMMatrixTranslation(r_, 0.0f, 0.0f) *
        dx::XMMatrixRotationRollPitchYaw(theta_, phi_, chi_) *
        dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
