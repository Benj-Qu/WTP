#include "Clock.hpp"

#include <time.h>

void Clock::reset() {
    this->time = clock();
}

bool Clock::exceed() {
    return ((double) (clock() - this->time) / CLOCKS_PER_SEC) > RETRANS_TIME;
}

