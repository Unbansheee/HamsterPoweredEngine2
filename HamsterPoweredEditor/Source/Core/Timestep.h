#pragma once

class Timestep
{
public:
    Timestep() : m_deltaTime(0), m_Time(0) {}
    
    void Update(float dt)
    {
        m_deltaTime = dt;
        m_Time += dt;
    }

    float GetSeconds() const
    {
        return m_deltaTime;
    }

    float GetMilliseconds() const
    {
        return m_deltaTime * 1000.0f;
    }

    float GetMinutes() const
    {
        return m_deltaTime / 60.0f;
    }

    float GetTimeAsSeconds() const
    {
        return m_Time;
    }

    float GetTimeAsMilliseconds() const
    {
        return m_Time * 1000.0f;
    }

    float GetTimeAsMinutes() const
    {
        return m_Time / 60.0f;
    }

private:
    float m_deltaTime;
    float m_Time;
};

