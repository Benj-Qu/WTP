#include "Clock.hpp"

void Clock::reset() {
    this->time = high_resolution_clock::now();
}

bool Clock::exceed() {
    return duration_cast<milliseconds>(high_resolution_clock::now() - this->time).count() >= RETRANS_TIME;
}

