#pragma once

#include "../Graphics.hpp"

#include <DirectXMath.h>

namespace fatpound::win32::d3d11::pipeline
{
    class Bindable;
    class IndexBuffer;
}

namespace fatpound::win32::d3d11::visual
{
    class Drawable
    {
        template <class C>
        friend class DrawableBase;

    public:
        Drawable() = default;
        Drawable(const Drawable& src) = delete;
        Drawable& operator = (const Drawable& src) = delete;
        Drawable(Drawable&& src) = delete;
        Drawable& operator = (Drawable&& src) = delete;
        virtual ~Drawable() = default;


    public:
        virtual auto GetTransformXM() const noexcept -> DirectX::XMMATRIX = 0;

        virtual void Update(float dt) noexcept = 0;

        virtual void Draw(Graphics& gfx) const noexcept(IN_RELEASE)final;


    protected:
        virtual void AddBind_(std::unique_ptr<NAMESPACE_PIPELINE::Bindable> bind) noexcept(IN_RELEASE)final;
        virtual void AddIndexBuffer_(std::unique_ptr<NAMESPACE_PIPELINE::IndexBuffer> idxbuf) noexcept(IN_RELEASE)final;


    private:
        virtual auto GetStaticBinds_() const noexcept -> const std::vector<std::unique_ptr<NAMESPACE_PIPELINE::Bindable>> & = 0;


    private:
        std::vector<std::unique_ptr<NAMESPACE_PIPELINE::Bindable>> binds_;

        const NAMESPACE_PIPELINE::IndexBuffer* pCIndexBuffer_ = nullptr;
    };
}