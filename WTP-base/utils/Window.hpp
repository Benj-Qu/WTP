#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <deque>
#include "Packet.hpp"

#define INIT_SEQ 0

class Window {
private:
    unsigned int size;
    unsigned int head;
    std::deque<Packet> packets;

public:
    Window() : size(0), head(0) {};
    Window(unsigned int _size) : size(_size), head(INIT_SEQ) {};
    bool full();
    bool empty();
    bool accept(unsigned int seqnum);
    void push(Packet packet);
    void pop();
    void acked(unsigned int seqnum);
    void forward();
    void cumulForward(unsigned int seqnum);
    void sendall(int sockfd, std::ofstream& log);
};

#endif