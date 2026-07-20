#pragma once
#include <Windows.h>
#include <string>

// Simple wrapper around a Win32 window.
// Owns the native window handle and manages the application's
// message loop.
class Window
{
public:

    // Creates a window with the given title and dimensions.
    Window(const std::wstring& title, int width, int height);

    // Destroys the window and releases its Win32 resources.
    ~Window();

    // A window represents a unique OS resource, so copying is disabled.
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Processes all pending Windows messages.
    // Returns false when the application should exit.
    bool ProcessMessages();

    // Accessors for the underlying window and its size.
    HWND GetHandle() const { return m_hwnd; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:

    // Used only during window creation to connect the Win32 window
    // with this C++ object.
    static LRESULT CALLBACK WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // Permanent window procedure that forwards messages to the
    // appropriate Window instance.
    static LRESULT CALLBACK WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // Handles all messages for this specific window instance.
    LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // Native Win32 window handle.
    HWND m_hwnd = nullptr;

    // Current client window dimensions.
    int m_width = 0;
    int m_height = 0;

    // Indicates when the application should stop running.
    bool m_shouldClose = false;

    // Name used to register the Win32 window class.
    static constexpr const wchar_t* CLASS_NAME = L"DX11EngineWindowClass";
};