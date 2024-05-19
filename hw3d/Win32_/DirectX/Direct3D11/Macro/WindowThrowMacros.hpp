#pragma once

#define FHWND_EXCEPT(hresult_)     ::fatpound::win32::d3d11::Window::HrException(__LINE__, __FILE__, hresult_)
#define FHWND_LAST_EXCEPT()        ::fatpound::win32::d3d11::Window::HrException(__LINE__, __FILE__, GetLastError())
#define FHWND_NOGFX_EXCEPT()       ::fatpound::win32::d3d11::Window::NoGfxException( __LINE__,__FILE__ )