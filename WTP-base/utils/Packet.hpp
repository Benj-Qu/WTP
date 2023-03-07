#ifndef __PACKET_H__
#define __PACKET_H__

#include "PacketHeader.hpp"

#define CHUNK_SIZE 1456
#define PACKET_SIZE 1472
#define HEADER_SIZE 16

class Packet {
public:
    unsigned int length;
    char content[PACKET_SIZE + 1];
    bool acked;
    Packet(unsigned int _length, PacketHeader* header, char* message);
    void extract(char* buffer);
};

#endif