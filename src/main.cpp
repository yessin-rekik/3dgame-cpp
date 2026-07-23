#include "core/Window.h"
#include "graphics/Direct3D.h"
#include <Windows.h>

int main()
{

    try
    {
        Window window(L"DirectX 11 Engine", 1280, 720);
        Direct3D d3d(window.GetHandle(), window.GetWidth(), window.GetHeight());

        while (window.ProcessMessages())
        {
            d3d.Clear(0.392f, 0.584f, 0.929f);

            // Actual drawing goes here (Step 2 onward).

            d3d.Present();
        }
    }
    catch (const std::exception &e)
    {
        MessageBoxA(nullptr, e.what(), "Fatal Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    return 0;
}
