#ifndef __PACKET_HEADER_H__
#define __PACKET_HEADER_H__

#include <fstream>

#define START 0
#define END 1
#define DATA 2
#define ACK 3

class PacketHeader
{
public:
    unsigned int type;     // 0: START; 1: END; 2: DATA; 3: ACK
    unsigned int seqNum;   // Described below
    unsigned int length;   // Length of data; 0 for ACK packets
    unsigned int checksum; // 32-bit CRC
    PacketHeader() : type(0), seqNum(0), length(0), checksum(0) {};
    PacketHeader(unsigned int _type, unsigned int _seqNum, unsigned int _length, unsigned int _checksum) : 
        type(_type), seqNum(_seqNum), length(_length), checksum(_checksum) {};
    PacketHeader(char* header);
    void encode(char* buffer);
    void log(std::ofstream& log);
};

#endif
