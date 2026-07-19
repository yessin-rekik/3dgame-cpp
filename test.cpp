
#include <Windows.h>

int CALLBACK WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, 
    int nCmdShow)
{

    const wchar_t* pClassName = L"hw3dbutts";
    
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(wc);

    wc.style = CS_OWNDC;
    wc.lpfnWndProc = DefWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = pClassName;

    RegisterClassEx(&wc);
    HWND hWnd = CreateWindowEx(
        0,
        pClassName,
        L"hw3dbutts",
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, hInstance, nullptr
    );

    ShowWindow (hWnd, SW_SHOW);

    while (true);
    return 0;
}