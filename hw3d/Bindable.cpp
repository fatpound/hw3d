#include "Bindable.hh"

ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx) noexcept
{
    return gfx.pContext_.Get();
}

ID3D11Device* Bindable::GetDevice(Graphics& gfx) noexcept
{
    return gfx.pDevice_.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(Graphics& gfx) noexcept(!IS_DEBUG)
{
#ifndef NDEBUG
    return gfx.infoManager_;
#else
    throw std::logic_error("Tried to access gfx.infoManager_ in Release config");
#endif
}