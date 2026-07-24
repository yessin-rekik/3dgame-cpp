#include "Timer.h"

Timer::Timer()
{
    QueryPerformanceFrequency(&m_frequency);
    QueryPerformanceCounter(&m_startTime);
    m_lastFrameTime = m_startTime;

}

float Timer::Tick()
{
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    const LONGLONG deltaTicks = now.QuadPart - m_lastFrameTime.QuadPart;
    const float deltaSeconds = static_cast<float>(deltaTicks) / static_cast<float>(m_frequency.QuadPart);

    m_lastFrameTime = now;
    return deltaSeconds;

}

float Timer::TotalTime()

{
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    const LONGLONG deltaTicks = now.QuadPart - m_startTime.QuadPart;
    const float deltaSeconds = static_cast<float>(deltaTicks) / static_cast<float>(m_frequency.QuadPart);

    return deltaSeconds;
}