#pragma once

#include "Graphics.hpp"

#include <DirectXMath.h>

class Bindable;

class Drawable
{
public:
    Drawable() = default;
    virtual ~Drawable() = default;
    Drawable(const Drawable& src) = delete;
    Drawable(Drawable&& src) = delete;
    Drawable& operator = (const Drawable& src) = delete;
    Drawable& operator = (Drawable&& src) = delete;


public:
    virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

    virtual void Update(float dt) noexcept = 0;
    void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
    void AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG);
    void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;


protected:


private:
    std::vector<std::unique_ptr<Bindable>> binds_;

    const IndexBuffer* pIndexBuffer_ = nullptr;
};