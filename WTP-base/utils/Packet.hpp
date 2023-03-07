#ifndef __PACKET_H__
#define __PACKET_H__

#include "PacketHeader.hpp"

#define CHUNK_SIZE 1456
#define PACKET_SIZE 1472
#define HEADER_SIZE 16

class Packet {
public:
    PacketHeader header;
    char data[CHUNK_SIZE + 1];
    bool acked;
    Packet(unsigned int type, unsigned int seqNum, unsigned int length, char* _data);
    Packet(char* buffer, std::ofstream& log);
    void send(int sockfd, std::ofstream& log);
    bool checkSum(std::ofstream& log);
};

#endif