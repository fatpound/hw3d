#include "SkinnedBox.hpp"

#include "../../Win32_/DirectX/Direct3D11/Bindable/BindableBase.hpp"
#include "../../Win32_/DirectX/Direct3D11/Bindable/Texture.hpp"

#include "../../Win32_/DirectX/Direct3D11/Macro/GraphicsThrowMacros.hpp"

#include "../../Win32_/GDI/Surface.hpp"

#include "Base/Cube.hpp"

namespace dx = DirectX;

SkinnedBox::SkinnedBox(Graphics& gfx,
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

        const auto model = Cube::MakeSkinned<Vertex>();

        DrawableBase::AddStaticBind_(std::make_unique<VertexBuffer>(gfx, model.vertices_));
        DrawableBase::AddStaticBind_(std::make_unique<Texture>(gfx, Surface::FromFile("Resource\\Image\\cube.png")));

        auto pvs = std::make_unique<VertexShader>(gfx, L"VSTexture.cso");
        auto pvsbc = pvs->GetBytecode();

        DrawableBase::AddStaticBind_(std::move(pvs));
        DrawableBase::AddStaticBind_(std::make_unique<PixelShader>(gfx, L"PSTexture.cso"));

        AddStaticIndexBuffer_(std::make_unique<IndexBuffer>(gfx, model.indices_));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
            { "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
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

void SkinnedBox::Update(float dt) noexcept
{
    roll_  += droll_  * dt;
    pitch_ += dpitch_ * dt;
    yaw_   += dyaw_   * dt;
    theta_ += dtheta_ * dt;
    phi_   += dphi_   * dt;
    chi_   += dchi_   * dt;
}

DirectX::XMMATRIX SkinnedBox::GetTransformXM() const noexcept
{
    return dx::XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_) *
        dx::XMMatrixTranslation(r_, 0.0f, 0.0f) *
        dx::XMMatrixRotationRollPitchYaw(theta_, phi_, chi_);
}