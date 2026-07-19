#pragma once
#include <Windows.h>
#include <string>

class Window
{
public:

    // Constructor
    Window(const std::wstring& title, int width, int height);

    // Destructor
    ~Window();

    // Forbids copying windows
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool ProcessMessages();

    HWND GetHandle() const { return m_hwnd; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    static LRESULT CALLBACK WindowProcSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WindowProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd = nullptr;
    int m_width = 0;
    int m_height = 0;
    bool m_shouldClose = false;

    static constexpr const wchar_t* CLASS_NAME = L"DX11EngineWindowClass";
};