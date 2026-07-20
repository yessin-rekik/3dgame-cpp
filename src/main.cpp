#include "core/Window.h"
#include "graphics/Direct3D.h"
#include <Windows.h>

int main()
{

    try
    {
        Window window(L"DirectX 11 Engine", 1280, 720);
        Direct3D d3d;

        while (window.ProcessMessages())
        {
            // Per-frame update/render logic will go here.
        }
    }
    catch (const std::exception &e)
    {
        MessageBoxA(nullptr, e.what(), "Fatal Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    return 0;
}
