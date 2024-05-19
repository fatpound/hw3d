#pragma once

#define D3D11_NAMESPACE                  ::fatpound::win32::d3d11
#define DXGI_NAMESPACE                   ::fatpound::win32::dxgi

///////////////////////////////////////////////////////////////////

#define GFX_EXCEPT_NOINFO(hr)            D3D11_NAMESPACE::Graphics::HrException( __LINE__, __FILE__, (hr) )
#define GFX_THROW_NOINFO(hrcall)         if (FAILED( hr = (hrcall) )) throw D3D11_NAMESPACE::Graphics::HrException(__LINE__, __FILE__, (hr))

#ifndef NDEBUG

#define GFX_EXCEPT(hr)                   D3D11_NAMESPACE::Graphics::HrException( __LINE__, __FILE__, (hr), infoManager_.GetMessages() )
#define GFX_THROW_INFO(hrcall)           infoManager_.SetNextIndex(); if (FAILED( hr = (hrcall) )) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr)    D3D11_NAMESPACE::Graphics::DeviceRemovedException( __LINE__, __FILE__, (hr), infoManager_.GetMessages() )
#define GFX_THROW_INFO_ONLY(call)        infoManager_.SetNextIndex(); (call); {auto v = infoManager_.GetMessages(); if(!v.empty()) {throw D3D11_NAMESPACE::Graphics::InfoException( __LINE__,__FILE__,v);}}
#define INFOMAN(gfx)                     HRESULT hr; DXGI_NAMESPACE::InfoManager& infoManager_ = GetInfoManager_((gfx))

#else

#define GFX_EXCEPT(hr)                   D3D11_NAMESPACE::Graphics::HrException( __LINE__, __FILE__, (hr) )
#define GFX_THROW_INFO(hrcall)           GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr)    D3D11_NAMESPACE::Graphics::DeviceRemovedException( __LINE__, __FILE__, (hr) )
#define GFX_THROW_INFO_ONLY(call)        (call)
#define INFOMAN(gfx)                     HRESULT hr

#endif