#pragma once

#define FHWND_EXCEPT(hresult_) Window::HrException(__LINE__, __FILE__, hresult_)
#define FHWND_LAST_EXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError())
#define FHWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )