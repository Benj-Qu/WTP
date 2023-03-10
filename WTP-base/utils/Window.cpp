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

void Window::senderForward() {
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

void Window::recverForward(std::ofstream& ofp) {
    while (!this->packets.empty()) {
        if (this->packets.front().acked) {
            ofp.write(this->packets.front().data, this->packets.front().header.length);
            this->packets.pop_front();
            this->head++;
        }
        else {
            break;
        }
    }
}

void Window::cumulForward(unsigned int seqnum) {
    if (!this->accept(seqnum)) return;
    while (!this->packets.empty()) {
        if (this->head >= seqnum) {
            break;
        }
        this->packets.pop_front();
        this->head++;
    }
}

void Window::sendall(AddrInfo* sender, std::ofstream& log) {
    for (Packet packet : this->packets) {
        packet.sendPack(sender, log);
    }
}