#include "Window.h"

// Creates a new window and registers the window class if needed.
Window::Window(const std::wstring& title, int width, int height)
    : m_width(width), m_height(height)
{
    // Get the current application's instance handle.
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    // Describe the type of window we're creating.
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;      // Redraw when resized horizontally or vertically.
    wc.lpfnWndProc = WindowProcSetup;        // Temporary message procedure used during creation.
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = CLASS_NAME;

    // Register the window class with Windows.
    RegisterClassEx(&wc);

    // Create the actual window.
    // Passing "this" lets us associate this Window object with the Win32 window.
    m_hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr,
        nullptr,
        hInstance,
        this
    );

    // Display the window if creation succeeded.
    if (m_hwnd)
    {
        ShowWindow(m_hwnd, SW_SHOW);
    }
}

// Cleans up the window when the object is destroyed.
Window::~Window()
{
    // Destroy the native window if it still exists.
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
    }

    // Remove the registered window class from the system.
    UnregisterClass(CLASS_NAME, GetModuleHandle(nullptr));
}

// Processes all pending Windows messages.
// Returns false once the application should exit.
bool Window::ProcessMessages()
{
    MSG msg = {};

    // Process every queued message before returning.
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        // WM_QUIT tells us the application is shutting down.
        if (msg.message == WM_QUIT)
        {
            m_shouldClose = true;
        }

        // Translate keyboard input and dispatch the message
        // to the window procedure.
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return !m_shouldClose;
}

// Initial window procedure.
// During WM_NCCREATE we receive the Window object pointer passed to CreateWindowEx.
LRESULT CALLBACK Window::WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* self = static_cast<Window*>(create->lpCreateParams);

        // Store the object pointer so future messages can find it.
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));

        // Replace this temporary procedure with the permanent one.
        SetWindowLongPtr(hwnd, GWLP_WNDPROC,
            reinterpret_cast<LONG_PTR>(&Window::WindowProcThunk));

        // Let the object handle the very first message.
        return self->HandleMessage(hwnd, msg, wParam, lParam);
    }

    // Until the object exists, use the default Windows behavior.
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Permanent window procedure.
// Simply looks up the associated Window object and forwards the message.
LRESULT CALLBACK Window::WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* self = reinterpret_cast<Window*>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));

    return self->HandleMessage(hwnd, msg, wParam, lParam);
}

// Member function that handles messages for this specific window instance.
LRESULT Window::HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        // Tell Windows the application is finished.
        PostQuitMessage(0);
        return 0;
    }

    // Let Windows handle any messages we don't care about.
    return DefWindowProc(hwnd, msg, wParam, lParam);
}