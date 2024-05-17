#pragma once

#include "../Graphics.hpp"

#include <DirectXMath.h>

class Bindable;
class IndexBuffer;

class Drawable
{
    template <class T>
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

    virtual void Draw(Graphics& gfx) const noexcept(IN_RELEASE) final;


protected:
    virtual void AddBind_(std::unique_ptr<Bindable> bind) noexcept(IN_RELEASE) final;
    virtual void AddIndexBuffer_(std::unique_ptr<IndexBuffer> ibuf) noexcept(IN_RELEASE) final;


private:
    virtual auto GetStaticBinds_() const noexcept -> const std::vector<std::unique_ptr<Bindable>>& = 0;


private:
    std::vector<std::unique_ptr<Bindable>> binds_;

    const IndexBuffer* pCIndexBuffer_ = nullptr;
};