//
// Created by Garry Morrison on 26/09/2020.
//

#ifndef SDB3_1_TIMER_H
#define SDB3_1_TIMER_H

#include <iostream>
#include <chrono>
#include <map>
#include <string>


const std::map<long long, std::string> time_interval_map = {  // time_interval_map assumes times are in milliseconds.
        { 1000 * 60 * 60 * 24 * 7, "week"},
        { 1000 * 60 * 60 * 24, "day"},
        { 1000 * 60 * 60, "hour"},
        { 1000 * 60, "minute"},
        { 1000, "second"},
        { 1, "millisecond"}
};


std::string display_time(const long long time);

// Copied from:
// VISUAL BENCHMARKING in C++ (how to measure performance visually)
// https://www.youtube.com/watch?v=xlAH4dbMVnU
// At the 3:00 mark.
//
class Timer_us {  // Currently records time in microseconds. For some applications we would prefer milliseconds. How handle?
private:
    const char* m_Name;
    // std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
    std::chrono::high_resolution_clock::time_point m_StartTimepoint;
    bool m_Stopped;
public:
    Timer_us(const char* name)
    : m_Name(name), m_Stopped(false) {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    void Stop() {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        std::cout << m_Name << ": " << (end - start) << " microseconds" << std::endl;
        m_Stopped = true;
    }

    ~Timer_us() {
        if (!m_Stopped) { Stop(); }
    }
};


class Timer_ms {
private:
    const char* m_Name;
    std::chrono::high_resolution_clock::time_point m_StartTimepoint;
    bool m_Stopped;
public:
    Timer_ms(const char* name)
            : m_Name(name), m_Stopped(false) {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    void Stop() {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimepoint).time_since_epoch().count();

        std::cout << m_Name << ": " << (end - start) << " milliseconds" << std::endl;
        m_Stopped = true;
    }

    ~Timer_ms() {
        if (!m_Stopped) { Stop(); }
    }
};


#endif //SDB3_1_TIMER_H
