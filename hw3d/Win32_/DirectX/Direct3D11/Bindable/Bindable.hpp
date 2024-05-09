#pragma once

#include "../Graphics.hpp"

class Bindable
{
public:
    virtual ~Bindable() = default;


public:
    virtual void Bind(Graphics& gfx) noexcept = 0;


protected:
    static ID3D11Device* GetDevice_(Graphics& gfx) noexcept;
    static ID3D11DeviceContext* GetContext_(Graphics& gfx) noexcept;
    static DxgiInfoManager& GetInfoManager_(Graphics& gfx) noexcept(IS_DEBUG);


private:
};