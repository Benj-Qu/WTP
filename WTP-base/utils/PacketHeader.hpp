#ifndef __PACKET_HEADER_H__
#define __PACKET_HEADER_H__

class PacketHeader
{
public:
    unsigned int type;     // 0: START; 1: END; 2: DATA; 3: ACK
    unsigned int seqNum;   // Described below
    unsigned int length;   // Length of data; 0 for ACK packets
    unsigned int checksum; // 32-bit CRC
    PacketHeader(char* header);
    void encode(char* buffer);
};

#endif
