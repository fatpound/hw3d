#include "Bindable.hpp"

namespace fatpound::win32::d3d11::pipeline
{
    ID3D11DeviceContext* Bindable::GetContext_(Graphics& gfx) noexcept
    {
        return gfx.pContext_.Get();
    }

    ID3D11Device* Bindable::GetDevice_(Graphics& gfx) noexcept
    {
        return gfx.pDevice_.Get();
    }

    NAMESPACE_DXGI::InfoManager& Bindable::GetInfoManager_([[maybe_unused]] Graphics& gfx) noexcept(IN_DEBUG)
    {
#ifndef NDEBUG
        return gfx.infoManager_;
#else
        throw std::logic_error("Tried to access gfx.infoManager_ in Release config");
#endif
    }
}