#include "Pyramid.hpp"

#include "Base/Cone.hpp"

namespace dx = DirectX;

using namespace fatpound::win32::d3d11;

namespace hw3d::obj
{
    Pyramid::Pyramid(ID3D11Device* const pDevice,
        std::minstd_rand& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist,
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

            auto model = base::Cone::MakeTesselated<Vertex>(4);

            // set vertex colors for mesh
            model.vertices_[0].color = { 255,255,0 };
            model.vertices_[1].color = { 255,255,0 };
            model.vertices_[2].color = { 255,255,0 };
            model.vertices_[3].color = { 255,255,0 };
            model.vertices_[4].color = { 255,255,80 };
            model.vertices_[5].color = { 255,10,0 };

            // deform mesh linearly
            model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 0.7f));

            DrawableBase::AddStaticBind_(std::make_unique<fatpound::win32::d3d11::pipeline::VertexBuffer>(pDevice, model.vertices_));

            auto pvs = std::make_unique<fatpound::win32::d3d11::pipeline::VertexShader>(pDevice, L"VSColorBlend.cso");
            auto pvsbc = pvs->GetBytecode();

            DrawableBase::AddStaticBind_(std::move<>(pvs));
            DrawableBase::AddStaticBind_(std::make_unique<fatpound::win32::d3d11::pipeline::PixelShader>(pDevice, L"PSColorBlend.cso"));
            AddStaticIndexBuffer_(std::make_unique<fatpound::win32::d3d11::pipeline::IndexBuffer>(pDevice, model.indices_));

            const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
            {
                { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "Color",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };

            DrawableBase::AddStaticBind_(std::make_unique<fatpound::win32::d3d11::pipeline::InputLayout>(pDevice, ied, pvsbc));
            DrawableBase::AddStaticBind_(std::make_unique<fatpound::win32::d3d11::pipeline::Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
        }
        else
        {
            SetIndexFromStatic_();
        }

        AddBind_(std::make_unique<fatpound::win32::d3d11::pipeline::TransformCBuffer<Pyramid>>(pDevice, *this, viewXM));
    }

    void Pyramid::Update(float deltaTime) noexcept
    {
        roll_ += droll_ * deltaTime;
        pitch_ += dpitch_ * deltaTime;
        yaw_ += dyaw_ * deltaTime;
        theta_ += dtheta_ * deltaTime;
        phi_ += dphi_ * deltaTime;
        chi_ += dchi_ * deltaTime;
    }

    auto Pyramid::GetTransformXM() const noexcept -> dx::XMMATRIX
    {
        return dx::XMMatrixRotationRollPitchYaw(pitch_, yaw_, roll_) *
            dx::XMMatrixTranslation(r_, 0.0f, 0.0f) *
            dx::XMMatrixRotationRollPitchYaw(theta_, phi_, chi_);
    }
}