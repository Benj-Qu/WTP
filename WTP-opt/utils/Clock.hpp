#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <chrono>

#define RETRANS_TIME 500

using namespace std::chrono;

class Clock {
private:
    high_resolution_clock::time_point time;

public:
    Clock() : time(high_resolution_clock::now()) {};
    void reset();
    bool exceed();
};

#endif