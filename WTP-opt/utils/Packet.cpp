#include "Packet.hpp"

#include <stdio.h>
#include <cstring>
#include <sys/socket.h>
#include "crc32.hpp"

Packet::Packet(unsigned int type, unsigned int seqNum, unsigned int length, char* _data) {
    this->header = PacketHeader(type, seqNum, length, crc32(_data, length));
    this->acked = false;
    memset(this->data, 0, sizeof(this->data));
    memcpy(this->data, _data, length);
}

Packet::Packet(char* buffer, std::ofstream& log) {
    this->header = PacketHeader(buffer);
    this->acked = false;
    memset(this->data, 0, sizeof(this->data));
    memcpy(this->data, buffer + HEADER_SIZE, this->header.length);
    this->header.log(log);
}

void Packet::sendPack(AddrInfo* sender, std::ofstream& log) {
    char message[PACKET_SIZE + 1];
    memset(message, 0, sizeof(message));
    this->header.encode(message);
    memcpy(message + HEADER_SIZE, this->data, this->header.length);
    if (sender->send(message, HEADER_SIZE + this->header.length) == -1) {
        perror("Fail Sending Packet");
        exit(1);
    }
    this->header.log(log);
}

bool Packet::checkSum() {
    return (crc32(this->data, this->header.length) == this->header.checksum);
}