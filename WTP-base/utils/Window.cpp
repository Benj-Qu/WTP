#include "Window.hpp"

bool Window::full() {
    return (this->packets.size() == this->size);
}

bool Window::empty() {
    return (this->packets.empty());
}

bool Window::accept(unsigned int seqnum) {
    return (seqnum <= (this->head + this->size - 1));
}

void Window::push(Packet packet) {
    this->packets.push_back(packet);
}

void Window::pop() {
    this->packets.pop_front();
    this->head++;
}

void Window::acked(unsigned int seqnum) {
    this->packets[seqnum-head].acked = true;
}

void Window::forward() {
    while (!this->packets.empty()) {
        if (this->packets.front().acked) {
            this->packets.pop_front();
            this->head++;
        }
        else {
            break;
        }
    }
}

void Window::cumulForward(unsigned int seqnum) {
    while (!this->packets.empty()) {
        if (this->head == seqnum) {
            break;
        }
        this->packets.pop_front();
        this->head++;
    }
}