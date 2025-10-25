//
// Created by kuba on 28.03.2025.
//

#include "Timer.h"

Timer::Timer() = default;

void Timer::reset() {
}

int Timer::start() {
    start_time = std::chrono::high_resolution_clock::now();
    return 0;
}

int Timer::stop() {
    end_time = std::chrono::high_resolution_clock::now();
    return 0;
}

int Timer::result() {
    auto duration = std::chrono::duration_cast<std::chrono::microseconds >(end_time-start_time);
    return static_cast<int>(duration.count());
}
