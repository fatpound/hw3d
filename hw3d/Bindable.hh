#pragma once

#include "Graphics.hpp"

class Bindable
{
public:
    virtual ~Bindable() = default;


public:
    virtual void Bind(Graphics& gfx) noexcept = 0;


protected:
    static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
    static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
    static DxgiInfoManager& GetInfoManager(Graphics& gfx) noexcept(! IS_DEBUG);


private:
};