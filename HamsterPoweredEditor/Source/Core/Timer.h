#pragma once
#include <chrono>
#include <iostream>

#define FUNC_PROFILE() Timer timer(__FUNCTION__)

class Timer
{
public:
    Timer(const std::string& name = "Timer")
    {
        m_startTime = std::chrono::system_clock::now();
        m_name = name;
    }
    ~Timer()
    {
        m_endTime = std::chrono::system_clock::now();

        auto us = std::chrono::duration_cast<std::chrono::microseconds>(m_endTime - m_startTime).count();
        auto ms = us * 0.001;
        std::cout << "--------------------\n";
        std::cout << "Timer: " << m_name  << std::endl;
        std::cout << "Elapsed Time: " << us << "us (" << ms << "ms)\n";
        std::cout << "--------------------\n";
    }
    

    std::chrono::time_point<std::chrono::system_clock> m_startTime;
    std::chrono::time_point<std::chrono::system_clock> m_endTime;
    std::string m_name;
};
