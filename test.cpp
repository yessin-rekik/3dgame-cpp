
#include <Windows.h>


//  window proc for handling messages
LRESULT CALLBACK WndProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        // close button clicked
        case WM_CLOSE:        
            PostQuitMessage( 69 );
            break;
    }

    // pass back to default window proc for default handling
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

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
    wc.lpfnWndProc = WndProc;
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

    // messages

    MSG msg;

    // check if result promps for ending program
    BOOL gResult;
    while ( (gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (gResult == -1)
    {
        return -1;
    } else {
        return msg.wParam;
    }
}