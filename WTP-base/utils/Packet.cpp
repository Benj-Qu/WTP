#include "Packet.hpp"

#include <stdio.h>
#include <cstring>
#include "utils/crc32.hpp"

Packet::Packet(unsigned int type, unsigned int seqNum, unsigned int length, char* _data) {
    this->header = PacketHeader(type, seqNum, length, crc32(_data, length));
    this->acked = false;
    memset(this->data, 0, sizeof(this->data));
    strncpy(this->data, _data, length);
}

Packet(char* buffer, std::ofstream& log) {
    this->header = PacketHeader(buffer);
    this->acked = false;
    memset(this->data, 0, sizeof(this->data));
    strncpy(this->data, buffer + HEADER_SIZE, this->header.length);
}

void Packet::send(int sockfd, std::ofstream& log) {
    char message[PACKET_SIZE + 1];
    memset(message, 0, sizeof(message));
    this->header.encode(message);
    strncpy(message + HEADER_SIZE, this->data, this->header.length);
    if (send(sockfd, message, PACKET_SIZE, 0) == -1) {
        perror("Fail Sending Packet");
        exit(1);
    }
    this->header.log(&log);
}

bool Packet::checkSum(std::ofstream& log) {
    if (crc32(this->data, this->header.length) == this->header.checksum) {
        this->header.log(&log);
        return true;
    }
    return false;
}