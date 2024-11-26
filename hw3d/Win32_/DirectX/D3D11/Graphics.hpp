#pragma once

#include <FatWin32.hpp>

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "../../../Math/Math.hpp"

#include "../../../Util/Surface.hpp"

#include "../../../Util/Gfx/Gfx.hpp"

#include "Core/Core.hpp"
#include "Factory/Factory.hpp"
#include "Pipeline/Pipeline.hpp"
#include "Visual/Visual.hpp"

#include "GraphicsResourcePack.hpp"

#include <d3d11.h>

#include <wrl.h>

#include <DirectXMath.h>

#include <array>
#include <memory>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "D3DCompiler")

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

using FATSPACE_UTIL::Color;
using FATSPACE_UTIL::Surface;
using FATSPACE_UTIL::ScreenSizeInfo;

namespace fatpound::win32::d3d11
{
	template <bool Framework = false>
    class Graphics final
    {
        static constexpr auto NotFramework = std::bool_constant<not Framework>::value;
        static constexpr auto RasterizationEnabled = NotFramework; // optional

        using ResourcePackType = std::conditional_t<Framework, GraphicsFrameworkResourcePack, GraphicsResourcePack>;
        using float_t = float;

    public:
        explicit Graphics(const HWND hWnd, const ScreenSizeInfo& dimensions)
            :
            mc_hWnd_(hWnd),
            mc_dimensions_{ dimensions }
        {
            InitCommon_();
            
            if constexpr (RasterizationEnabled)
            {
                InitRasterizer_();
            }

            ImGui_ImplDX11_Init(GetDevice(), GetImmediateContext());
        }
        explicit Graphics(const HWND hWnd, const ScreenSizeInfo& dimensions) requires(Framework)
            :
            m_res_pack_(dimensions),
            mc_hWnd_(hWnd),
            mc_dimensions_{ dimensions }
        {
            InitCommon_();
            InitFramework_();

            ImGui_ImplDX11_Init(GetDevice(), GetImmediateContext());
        }
        explicit Graphics(const HWND hWnd, std::unique_ptr<Surface> pSurface) requires(Framework)
            :
            Graphics(hWnd, pSurface->GetScreenSizeInfo())
        {
            BindSurface(std::move(pSurface));
        }

        explicit Graphics() = delete;
        explicit Graphics(const Graphics& src) = delete;
        explicit Graphics(Graphics&& src) = delete;

        auto operator = (const Graphics& src) -> Graphics& = delete;
        auto operator = (Graphics&& src)      -> Graphics& = delete;
        ~Graphics() noexcept
        {
            ImGui_ImplDX11_Shutdown();
        }
        ~Graphics() noexcept requires(Framework)
        {
            if (m_res_pack_.m_pImmediateContext not_eq nullptr) [[likely]]
            {
                try
                {
                    m_res_pack_.m_pImmediateContext->ClearState();
                }
                catch (...)
                {
                    OutputDebugString(L"COM Exception caught in Graphics<Framework> destructor!\n");
                }
            }

            ImGui_ImplDX11_Shutdown();
        }


    public:
        template <FATSPACE_MATH::number_set::Rational Q> auto GetWidth()  const noexcept
        {
            return static_cast<Q>(mc_dimensions_.m_width);
        }
        template <FATSPACE_MATH::number_set::Rational Q> auto GetHeight() const noexcept
        {
            return static_cast<Q>(mc_dimensions_.m_height);
        }

        template <bool FullBlack = true, float_t red = 1.0f, float_t green = 1.0f, float_t blue = 1.0f, float_t alpha = 1.0f>
        void BeginFrame() requires(NotFramework)
        {
            if (IsImguiEnabled())
            {
                ::ImGui_ImplDX11_NewFrame();
                ::ImGui_ImplWin32_NewFrame();

                ::ImGui::NewFrame();
            }

            if constexpr (FullBlack)
            {
                FillWithSolidColor<>();
            }
            else
            {
                FillWithSolidColor<red, green, blue, alpha>();
            }
        }

        template <int GrayToneValue = 0>
        void BeginFrame() noexcept requires(Framework)
        {
            [[maybe_unused]]
            void* const ptr = ::std::memset(
                m_res_pack_.m_surface,
                GrayToneValue,
                sizeof(Color) * mc_dimensions_.m_width * mc_dimensions_.m_height
            );
        }

        template <bool VSynced = true>
        void EndFrame()
        {
            if (IsImguiEnabled())
            {
                ::ImGui::Render();

                ::ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            }

            if constexpr (Framework)
            {
                MapSubresource_();
                CopySysbufferToMappedSubresource_();

                GetImmediateContext()->Unmap(GetSysbufferTexture(), 0u);
                GetImmediateContext()->Draw(6u, 0u);
            }

            const auto& hr = m_res_pack_.m_pSwapChain->Present(static_cast<UINT>(VSynced), 0u);

            if (FAILED(hr)) [[unlikely]]
            {
                throw std::exception("SwapChain could NOT Present!");;
            }
        }

        template <float_t red = 0.0f, float_t green = 0.0f, float_t blue = 0.0f, float_t alpha = 1.0f>
        void FillWithSolidColor() requires(NotFramework)
        {
            constexpr std::array<const float_t, 4> colors{ red, green, blue, alpha };

            m_res_pack_.m_pImmediateContext->ClearRenderTargetView(m_res_pack_.m_pRTV.Get(), colors.data());
            m_res_pack_.m_pImmediateContext->ClearDepthStencilView(m_res_pack_.m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
        }

        template <std::floating_point T = float_t>
        void FillWithSolidColor(const T red, const T green, const T blue, const T alpha = static_cast<T>(1.0)) requires(NotFramework)
        {
            const std::array<const T, 4> colors{ red, green, blue, alpha };

            m_res_pack_.m_pImmediateContext->ClearRenderTargetView(m_res_pack_.m_pRTV.Get(), colors.data());
            m_res_pack_.m_pImmediateContext->ClearDepthStencilView(m_res_pack_.m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
        }


    public:
        auto IsImguiEnabled() const noexcept -> bool
        {
            return m_imgui_enabled_;
        }

        auto GetHwnd() const -> HWND
        {
            return mc_hWnd_;
        }
        auto GetDevice() const noexcept -> ID3D11Device*
        {
            return m_res_pack_.m_pDevice.Get();
        }
        auto GetImmediateContext() const noexcept -> ID3D11DeviceContext*
        {
            return m_res_pack_.m_pImmediateContext.Get();
        }
        auto GetRenderTargetView() const noexcept -> ID3D11RenderTargetView*
        {
            return m_res_pack_.m_pRTV.Get();
        }
        auto GetDepthStencilView() const noexcept -> ID3D11DepthStencilView*
        {
            return m_res_pack_.m_pDSV.Get();
        }
        auto GetSysbufferTexture() const noexcept -> ID3D11Texture2D* requires(Framework)
        {
            return m_res_pack_.m_pSysBufferTexture.Get();
        }

        void EnableImgui() noexcept;
        void DisableImgui() noexcept;

        void BindSurface(std::unique_ptr<Surface> pSurface) requires(Framework)
        {
            if (m_extra_pSurface_ not_eq nullptr)
            {
                m_extra_pSurface_->Clear();
            }

            m_extra_pSurface_ = std::move(pSurface);
        }
        void CopySurfaceToSysBuffer() requires(Framework)
        {
            const void* const pSrc = *(m_extra_pSurface_.get());

            if (pSrc not_eq nullptr)
            {
                ::std::memcpy(
                    static_cast<void*>(m_res_pack_.m_surface),
                    pSrc,
                    sizeof(Color) * mc_dimensions_.m_width * mc_dimensions_.m_height
                );
            }
        }

        void PutPixel(const int x, const int y, const Color color) noexcept requires(Framework)
        {
            assert(x >= 0);
            assert(x < static_cast<int>(mc_dimensions_.m_width));
            assert(y >= 0);
            assert(y < static_cast<int>(mc_dimensions_.m_height));

            m_res_pack_.m_surface[mc_dimensions_.m_width * y + x] = color;
        }


    protected:


    private:
        void InitCommon_()
        {
            core::Create_Device(m_res_pack_);

            InitMSAA_Settings_();
            
            {
                auto&& scdesc = factory::Create_SwapChain_DESC<Framework>(GetHwnd(), mc_dimensions_, m_msaa_count_, m_msaa_quality_);
                factory::Create_SwapChain(m_res_pack_, scdesc);
            }

            ToggleAltEnterMode_();

            InitRenderTarget_();
            InitViewport_();
        }
        void InitFramework_() requires(Framework)
        {
            InitShaderResourceView_();
            InitSampler_();

            std::vector<std::unique_ptr<FATSPACE_PIPELINE::Bindable>> binds;

            InitFrameworkBinds_(binds);

            auto* const pImmediateContext = GetImmediateContext();

            for (auto& bindable : binds)
            {
                bindable->Bind(pImmediateContext);
            }
        }
        void InitFrameworkBinds_(std::vector<std::unique_ptr<FATSPACE_PIPELINE::Bindable>>& binds) requires(Framework)
        {
            auto pVS = std::make_unique<FATSPACE_PIPELINE_ELEMENT::VertexShader>(GetDevice(), L"..\\FatModules\\VSFrameBuffer.cso");
            auto pBlob = pVS->GetBytecode();

            binds.push_back(std::move(pVS));
            binds.push_back(std::make_unique<FATSPACE_PIPELINE_ELEMENT::PixelShader>(GetDevice(), L"..\\FatModules\\PSFrameBuffer.cso"));
            binds.push_back(std::make_unique<FATSPACE_PIPELINE_ELEMENT::VertexBuffer>(GetDevice(), FATSPACE_UTIL_GFX::FullScreenQuad::sc_vertices));
            binds.push_back(std::make_unique<FATSPACE_PIPELINE_ELEMENT::Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

            const std::vector<D3D11_INPUT_ELEMENT_DESC> iedesc =
            {
                {
                    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
                }
            };

            binds.push_back(std::make_unique<FATSPACE_PIPELINE_ELEMENT::InputLayout>(GetDevice(), iedesc, pBlob));
        }
        void InitMSAA_Settings_()
        {
            constexpr std::array<const UINT, 4> msaa_counts{ 32u, 16u, 8u, 4u };

            for (auto i = 0u; i < msaa_counts.size(); ++i)
            {
                m_res_pack_.m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM, msaa_counts[i], &m_msaa_quality_);

                if (m_msaa_quality_ > 0)
                {
                    m_msaa_count_ = msaa_counts[i];

                    break;
                }
            }

            if (m_msaa_quality_ <= 0)
            {
                throw std::runtime_error{ "MSAA Quality is NOT valid!" };
            }
        }

        void InitRasterizer_() requires(NotFramework)
        {
            ::wrl::ComPtr<ID3D11RasterizerState> pRasterizerState{};

            {
                const auto& descRS = factory::Create_RasterizerState_DESC();
                factory::Create_RasterizerState(m_res_pack_, descRS, pRasterizerState);
            }

            GetImmediateContext()->RSSetState(pRasterizerState.Get());
        }
        void InitRenderTarget_()
        {
            factory::Create_RenderTargetView(m_res_pack_);

            if constexpr (NotFramework)
            {
                ::wrl::ComPtr<ID3D11Texture2D> pTexture2D{};

                {
                    const auto& descTex2D = factory::Create_Texture2D_DESC(mc_dimensions_, m_msaa_count_, m_msaa_quality_);
                    factory::Create_Texture2D(GetDevice(), descTex2D, pTexture2D);
                }

                {
                    const auto& descDSV = factory::Create_DepthStencilView_DESC(m_msaa_count_);
                    factory::Create_DepthStencilView(m_res_pack_, pTexture2D, descDSV);
                }
            }

            GetImmediateContext()->OMSetRenderTargets(1u, m_res_pack_.m_pRTV.GetAddressOf(), GetDepthStencilView());
        }
        void InitViewport_()
        {
            D3D11_VIEWPORT vp{};
            vp.Width = static_cast<FLOAT>(mc_dimensions_.m_width);
            vp.Height = static_cast<FLOAT>(mc_dimensions_.m_height);
            vp.MinDepth = 0.0f;
            vp.MaxDepth = 1.0f;
            vp.TopLeftX = 0.0f;
            vp.TopLeftY = 0.0f;

            GetImmediateContext()->RSSetViewports(1u, &vp);
        }
        void InitShaderResourceView_() requires(Framework)
        {
            ::wrl::ComPtr<ID3D11ShaderResourceView> pSysBufferTextureView{};

            {
                const auto& t2dDesc = factory::Create_Texture2D_DESC<Framework>(mc_dimensions_, m_msaa_count_, m_msaa_quality_);
                factory::Create_Texture2D(m_res_pack_, t2dDesc);

                const auto& srvDesc = factory::Create_ShaderResourceView_DESC<Framework>(t2dDesc.Format, m_msaa_count_);
                factory::Create_ShaderResourceView(m_res_pack_, srvDesc, pSysBufferTextureView);
            }

            GetImmediateContext()->PSSetShaderResources(0u, 1u, pSysBufferTextureView.GetAddressOf());
        }
        void InitSampler_() requires(Framework)
        {
            ::wrl::ComPtr<ID3D11SamplerState> pSamplerState{};

            {
                const auto& sampDesc = factory::Create_SamplerState_DESC();
                factory::Create_SamplerState(m_res_pack_, sampDesc, pSamplerState);
            }

            GetImmediateContext()->PSSetSamplers(0u, 1u, pSamplerState.GetAddressOf());
        }

        void MapSubresource_() requires(Framework)
        {
            const auto& hr = GetImmediateContext()->Map(
                GetSysbufferTexture(),
                0u,
                D3D11_MAP_WRITE_DISCARD,
                0u,
                &m_res_pack_.m_mappedSysBufferTexture
            );

            if (FAILED(hr)) [[unlikely]]
            {
                throw std::exception("Could NOT Map the ImmediateContext!");
            }
        }
        void CopySysbufferToMappedSubresource_() requires(Framework)
        {
            Color* const pDst = static_cast<Color*>(m_res_pack_.m_mappedSysBufferTexture.pData);

            const auto dstPitch = m_res_pack_.m_mappedSysBufferTexture.RowPitch / sizeof(Color);
            const auto srcPitch = mc_dimensions_.m_width;
            const auto rowBytes = srcPitch * sizeof(Color);

            for (auto y = 0u; y < mc_dimensions_.m_height; ++y)
            {
                std::memcpy(
                    static_cast<void*>(&pDst[y * dstPitch]),
                    static_cast<void*>(&m_res_pack_.m_surface[y * srcPitch]),
                    rowBytes
                );
            }
        }

        void ToggleAltEnterMode_()
        {
            FATSPACE_UTIL::gfx::ToggleDXGI_AltEnterMode(GetDevice(), GetHwnd(), m_dxgi_mode_);
        }


    private:
        ResourcePackType m_res_pack_{};

        const HWND mc_hWnd_;
        
        const ScreenSizeInfo mc_dimensions_;

        UINT m_msaa_count_{};
        UINT m_msaa_quality_{};

        UINT m_dxgi_mode_{};

        bool m_imgui_enabled_ = true;

        std::unique_ptr<Surface> m_extra_pSurface_;
    };
}