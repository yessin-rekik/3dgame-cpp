#pragma once
#include <Windows.h>

// High-resolution frame timer using QueryPerformanceCounter (QPC).
//
// Why QPC instead of GetTickCount/timeGetTime: QPC has microsecond-level
// resolution and isn't tied to the OS's coarse system timer, which matters
// once we're computing per-frame delta time for movement/animation - a
// jittery timer directly means jittery, inconsistent-feeling motion.

class Timer 
{
public:
    Timer();

    float Tick();

    float TotalTime();

private:
    LARGE_INTEGER m_startTime;
    LARGE_INTEGER m_lastFrameTime;
    LARGE_INTEGER m_frequency;
};