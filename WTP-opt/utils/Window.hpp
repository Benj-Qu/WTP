#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <deque>
#include "Packet.hpp"
#include "Clock.hpp"
#include "udp.hpp"

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
    void fill();
    void reset();
    void ack(unsigned int seqNum);
    void receive(Packet packet);
    void senderForward();
    void recverForward(std::ofstream& ofp);
    void cumulForward(unsigned int seqnum);
    void sendall(AddrInfo* sender, std::ofstream& log);
    void sendAck(AddrInfo* sender, std::ofstream& log);
    void resendAll(AddrInfo* sender, std::ofstream& log);
};

#endif