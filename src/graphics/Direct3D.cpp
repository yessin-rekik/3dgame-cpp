#include "Direct3D.h"
#include <stdexcept> // Provides std::runtime_error for reporting initialization failures.

// Constructs and initializes the core Direct3D objects.
Direct3D::Direct3D()
{
    // Flags that control how the Direct3D device is created.
    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    // Enable the Direct3D debug layer when building in Debug mode.
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // List of Direct3D feature levels we're willing to accept, ordered highest to lowest.
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    // Receives the highest feature level supported by the hardware.
    D3D_FEATURE_LEVEL chosenFeatureLevel;

    // Create the Direct3D device and its immediate context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                  // Use the default graphics adapter.
        D3D_DRIVER_TYPE_HARDWARE, // Use the physical GPU instead of a software renderer.
        nullptr,                  // No software rasterizer module.
        createDeviceFlags,        // Device creation options.
        featureLevels,            // Preferred feature levels.
        ARRAYSIZE(featureLevels), // Number of feature levels provided.
        D3D11_SDK_VERSION,        // Ensure the application matches the installed D3D runtime.
        m_device.GetAddressOf(),  // Receives the created device.
        &chosenFeatureLevel,      // Receives the feature level that was actually selected.
        m_context.GetAddressOf()  // Receives the immediate device context.
    );

    // Abort initialization if Direct3D failed to create a usable device.
    if (FAILED(hr))
    {
        throw std::runtime_error("D3D11CreateDevice failed - no compatible GPU/driver found");
    }
}