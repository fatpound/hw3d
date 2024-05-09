#include "Sheet.hpp"

#include "../../Win32_/DirectX/Direct3D11/Bindable/BindableBase.hpp"
#include "../../Win32_/DirectX/Direct3D11/Bindable/Texture.hpp"
#include "../../Win32_/DirectX/Direct3D11/Bindable/Sampler.hpp"

#include "../../Win32_/DirectX/Direct3D11/Macro/GraphicsThrowMacros.hpp"

#include "../../Win32_/GDI/Surface.hpp"

#include "Base/Plane.hpp"

namespace dx = DirectX;

Sheet::Sheet(Graphics& gfx,
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

        auto model = Plane::Make<Vertex>();
        model.vertices_[0].tex = { 0.0f,0.0f };
        model.vertices_[1].tex = { 1.0f,0.0f };
        model.vertices_[2].tex = { 0.0f,1.0f };
        model.vertices_[3].tex = { 1.0f,1.0f };

        AddStaticBind_(std::make_unique<Texture>(gfx, Surface::FromFile("Resource\\Image\\kappa50.png")));

        AddStaticBind_(std::make_unique<VertexBuffer>(gfx, model.vertices_));

        AddStaticBind_(std::make_unique<Sampler>(gfx));

        auto pvs = std::make_unique<VertexShader>(gfx, L"VSTexture.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind_(std::move(pvs));

        AddStaticBind_(std::make_unique<PixelShader>(gfx, L"PSTexture.cso"));

        AddStaticIndexBuffer_(std::make_unique<IndexBuffer>(gfx, model.indices_));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        AddStaticBind_(std::make_unique<InputLayout>(gfx, ied, pvsbc));

        AddStaticBind_(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexFromStatic_();
    }

    AddBind(std::make_unique<TransformCbuf>(gfx, *this));
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

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept
{
    return dx::XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_) *
        dx::XMMatrixTranslation(r_, 0.0f, 0.0f) *
        dx::XMMatrixRotationRollPitchYaw(theta_, phi_, chi_);
}
