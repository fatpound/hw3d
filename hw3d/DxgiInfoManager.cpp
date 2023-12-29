#include "DxgiInfoManager.hpp"
#include "Window.hpp"
#include "Graphics.hpp"

#include <memory>

#pragma comment(lib, "dxguid")

#define GFX_THROW_NOINFO(hrcall) if (FAILED( hr = (hrcall) )) throw Graphics::HrException( __LINE__, __FILE__, (hr) )

DxgiInfoManager::DxgiInfoManager()
{
    typedef HRESULT(WINAPI *DXGIGetDebugInterface)(REFIID, void**);

    const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

    if (hModDxgiDebug == nullptr)
    {
        throw FHWND_LAST_EXCEPT();
    }

    const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
        reinterpret_cast<void*>(
            GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
    );

    if (DxgiGetDebugInterface == nullptr)
    {
        throw FHWND_LAST_EXCEPT();
    }

    HRESULT hr;

    GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &pDxgiInfoQueue));
}


void DxgiInfoManager::SetNextIndex() noexcept
{
    next_error_index_ = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const
{
    std::vector<std::string> messages;

    const auto endIndex = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

    for (auto i = next_error_index_; i < endIndex; i++)
    {
        HRESULT hr;
        SIZE_T messageLength = 0;

        GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

        auto msgBytes = std::make_unique<byte[]>(messageLength);
        auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(msgBytes.get());

        GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));

        messages.emplace_back(pMessage->pDescription);
    }

    return messages;
}
