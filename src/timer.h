#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <cstdint>

class Timer {
    public:
        using Nanos = std::chrono::nanoseconds;
        using Micros = std::chrono::microseconds;
        using Millis = std::chrono::milliseconds;
        using Seconds = std::chrono::seconds;
        using Minutes = std::chrono::minutes;
        using Hours = std::chrono::hours;

        Timer() : t1(std::chrono::steady_clock::now()) {
        }

        void restart() {
            t1 = std::chrono::steady_clock::now();
        }

        template <typename T> 
        uint64_t click() {
            auto t2 = std::chrono::steady_clock::now();
            auto timeElapsed = std::chrono::duration_cast<T>(t2 - t1).count();
            t1 = t2;
            return timeElapsed;
        }
        template <typename T> 
        uint64_t glance() const {
            auto t2 = std::chrono::steady_clock::now();
            return std::chrono::duration_cast<T>(t2 - t1).count();
        }
    
    private:
        std::chrono::time_point<std::chrono::steady_clock> t1;
};

#endif  // TIMER_H
