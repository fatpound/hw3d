#include "Box.hh"
#include "BindableBase.hh"
#include "GraphicsThrowMacros.hh"
#include "Sphere.hh"

namespace dx = DirectX;

Box::Box(Graphics& gfx,
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
    if (! IsStaticInitialized())
    {
        struct Vertex
        {
            dx::XMFLOAT3 pos;
        };
        
        auto model = Sphere::Make<Vertex>();

        model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 1.2f));

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices_));

        auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices_));

        struct ConstantBuffer2
        {
            struct
            {
                float r;
                float g;
                float b;
                float a;
            }
            face_colors[6];
        };

        const ConstantBuffer2 cb2 =
        {
            {
                { 1.0f, 0.0f, 1.0f },
                { 1.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f },
                { 1.0f, 1.0f, 0.0f },
                { 0.0f, 1.0f, 1.0f }
            }
        };

        AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));
        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

void Box::Update(float deltaTime) noexcept
{
    roll_  += droll_  * deltaTime;
    pitch_ += dpitch_ * deltaTime;
    yaw_   += dyaw_   * deltaTime;
    theta_ += dtheta_ * deltaTime;
    phi_   += dphi_   * deltaTime;
    chi_   += dchi_   * deltaTime;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
    return DirectX::XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_) *
        DirectX::XMMatrixTranslation(r_, 0.0f, 0.0f) *
        DirectX::XMMatrixRotationRollPitchYaw(theta_, phi_, chi_) *
        DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
