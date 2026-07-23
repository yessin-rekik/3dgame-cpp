#pragma once

#include <d3d11.h>
#include <wrl/client.h> // Provides ComPtr, a smart pointer for COM objects.

class Direct3D
{
public:
    // Initializes the Direct3D device and immediate context.
    Direct3D(HWND hwnd, int width, int height);

    // Clears the back buffer to a solid color. Call once per frame, before
    // any drawing (once we have drawing).
    void Clear(float r, float g, float b, float a = 1.0f);

    // Flips the back buffer to the screen. Call once per frame, after
    // all drawing for that frame is done.
    void Present();


    // Returns the device used to create GPU resources like buffers and textures.
    ID3D11Device* GetDevice() const { return m_device.Get(); }

    // Returns the context used to issue rendering commands to the GPU.
    ID3D11DeviceContext* GetContext() const { return m_context.Get(); }

    IDXGISwapChain* GetSwapChain() const { return m_swapChain.Get(); }

    ID3D11RenderTargetView* GetRenderTargetView() const { return m_renderTargetView.Get(); }

    
private:

    void CreateRenderTargetView();
    void SetViewport(int width, int height);

    // Owns the Direct3D device and automatically releases it when destroyed.
    Microsoft::WRL::ComPtr<ID3D11Device> m_device;

    // Owns the immediate device context used for rendering.
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;

    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
};