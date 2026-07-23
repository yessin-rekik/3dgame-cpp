#include "Direct3D.h"
#include <stdexcept>

Direct3D::Direct3D(HWND hwnd, int width, int height)
{
    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1;
    scd.OutputWindow = hwnd;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL chosenFeatureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &scd,
        m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(),
        &chosenFeatureLevel,
        m_context.GetAddressOf()
    );

    if (FAILED(hr))
    {
        throw std::runtime_error("D3D11CreateDeviceAndSwapChain failed - no compatible GPU/driver found");
    }

    CreateRenderTargetView();
}

void Direct3D::CreateRenderTargetView()
{
    // The swap chain owns a back buffer texture (ID3D11Texture2D) - we don't
    // create it ourselves, we just ask the swap chain for it, then wrap it
    // in a render target view so the pipeline knows how to write to it.
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to get swap chain back buffer");
    }

    hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create render target view");
    }

    // backBuffer (the ComPtr) goes out of scope here and releases its
    // reference automatically - the RTV holds its own reference internally,
    // so the underlying texture stays alive as long as we need it.
}