#include "Direct3D.h"
#include <stdexcept>

#include <iostream> // Required header

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
    SetViewport(width, height);
    CreateRasterizerState();
}


void Direct3D::CreateRasterizerState()
{
    // TEMPORARY: CullMode = NONE means every triangle draws regardless of
    // winding order, front-facing or not. D3D11's default is
    // D3D11_CULL_BACK, which silently discards "back-facing" triangles -
    // but "facing" is only meaningful once there's an actual camera/view
    // matrix establishing a viewpoint, which we don't have yet. Without one, our winding assumptions don't
    // resolve the way intended, and backface culling was discarding all
    // 12 of the cube's triangles - hence nothing rendering at all.
    //
    // Once a real camera exists, we'll revisit this, verify winding is
    // actually correct from the camera's point of view, and switch back
    // to D3D11_CULL_BACK (culling is a real performance win - roughly
    // half of any closed mesh's triangles face away from the camera at
    // any given time and are wasted pixel shader work if not discarded).
    D3D11_RASTERIZER_DESC desc = {};
    desc.FillMode = D3D11_FILL_SOLID;
    desc.CullMode = D3D11_CULL_NONE;
    desc.FrontCounterClockwise = FALSE;
    desc.DepthClipEnable = TRUE;
 
    HRESULT hr = m_device->CreateRasterizerState(&desc, m_rasterizerState.GetAddressOf());
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create rasterizer state");
    }
 
    m_context->RSSetState(m_rasterizerState.Get());
}

void Direct3D::CreateRenderTargetView()
{
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
}

void Direct3D::SetViewport(int width, int height)
{
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_context->RSSetViewports(1, &viewport);
}

void Direct3D::Clear(float r, float g, float b, float a)
{
    ID3D11RenderTargetView* rtv = m_renderTargetView.Get();
    m_context->OMSetRenderTargets(1, &rtv, nullptr);

    const float clearColor[4] = { r, g, b, a };
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
}

void Direct3D::Resize(int width, int height)
{
    if (width <= 0 || height <= 0)
    {
        return; // minimized or degenerate size - nothing sensible to do
    }
 
    // Unbind the render target before releasing it - the swap chain refuses
    // to resize its buffers while anything (including the pipeline itself)
    // still holds a live reference to the old ones.
    m_context->OMSetRenderTargets(0, nullptr, nullptr);
    m_renderTargetView.Reset();
    
 
    // 0 = keep the existing buffer count; DXGI_FORMAT_UNKNOWN = keep the
    // existing format. We're only changing dimensions here.
    HRESULT hr = m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to resize swap chain buffers");
    }
 
    // The old render target view pointed at the now-destroyed back buffer -
    // rebuild it against the freshly resized one.
    CreateRenderTargetView();
    SetViewport(width, height);
}

void Direct3D::Present()
{
    m_swapChain->Present(1, 0);
}