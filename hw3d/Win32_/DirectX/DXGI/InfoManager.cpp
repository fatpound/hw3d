#include "InfoManager.hpp"

#include "../../Window.hpp"

#include "../Direct3D11/Graphics.hpp"

#include "../Direct3D11/Macro/GraphicsThrowMacros.hpp"
#include "../Direct3D11/Macro/WindowThrowMacros.hpp"

#include <memory>

#pragma comment(lib, "dxguid")

namespace fatpound::win32::dxgi
{
    InfoManager::InfoManager()
    {
        typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

        const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

        if (hModDxgiDebug == nullptr)
        {
            throw FHWND_LAST_EXCEPT();
        }

        const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
            reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
        );

        if (DxgiGetDebugInterface == nullptr)
        {
            throw FHWND_LAST_EXCEPT();
        }

        HRESULT hr;

        GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &pDxgiInfoQueue_));
    }


    void InfoManager::SetNextIndex() noexcept
    {
        next_error_index_ = pDxgiInfoQueue_->GetNumStoredMessages(DXGI_DEBUG_ALL);
    }

    auto InfoManager::GetMessages() const -> std::vector<std::string>
    {
        std::vector<std::string> messages;

        const auto endIndex = pDxgiInfoQueue_->GetNumStoredMessages(DXGI_DEBUG_ALL);

        for (auto i = next_error_index_; i < endIndex; ++i)
        {
            HRESULT hr;
            SIZE_T messageLength = 0;

            GFX_THROW_NOINFO(pDxgiInfoQueue_->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

            auto msgBytes = std::make_unique<byte[]>(messageLength);
            auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(msgBytes.get());

            GFX_THROW_NOINFO(pDxgiInfoQueue_->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));

            messages.emplace_back(pMessage->pDescription);
        }

        return messages;
    }
}