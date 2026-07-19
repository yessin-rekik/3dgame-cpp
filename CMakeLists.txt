cmake_minimum_required(VERSION 3.20)
project(DirectX11Engine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT WIN32)
    message(FATAL_ERROR "This project targets Windows (Win32 + DirectX 11) only.")
endif()

# Plain console-subsystem executable for now (no WIN32/-mwindows flag).
# This keeps a console window attached automatically, which is handy for
# printf/debug output while working through Step 0-2 where bugs are often
# silent (black screen, no errors). We can switch to a windowed subsystem
# later once we don't need the console anymore.
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/core/Window.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
)

# No file extensions here - CMake resolves these correctly for both
# MSVC (d3d11.lib) and MinGW-w64 (libd3d11.a) automatically.
target_link_libraries(${PROJECT_NAME} PRIVATE
    d3d11
    dxgi
    d3dcompiler
)
