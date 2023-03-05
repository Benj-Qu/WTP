#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <time.h>

#define RETRANS_TIME 0.5 // seconds

class Clock {
private:
    clock_t time;

public:
    Clock() : time(clock()) {};
    void reset();
    bool exceed();
};

#endif