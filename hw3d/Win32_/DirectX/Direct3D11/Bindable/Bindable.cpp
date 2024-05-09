#include "Bindable.hpp"

ID3D11DeviceContext* Bindable::GetContext_(Graphics& gfx) noexcept
{
    return gfx.pContext_.Get();
}

ID3D11Device* Bindable::GetDevice_(Graphics& gfx) noexcept
{
    return gfx.pDevice_.Get();
}

DxgiInfoManager& Bindable::GetInfoManager_(Graphics& gfx) noexcept(IN_DEBUG)
{
#ifndef NDEBUG
    return gfx.infoManager_;
#else
    throw std::logic_error("Tried to access gfx.infoManager_ in Release config");
#endif
}