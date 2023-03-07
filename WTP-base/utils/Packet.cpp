#include "Packet.hpp"

#include <stdio.h>
#include <cstring>

Packet::Packet(unsigned int _length, PacketHeader* header, char* message) {
    this->length = _length;
    this->acked = false;
    // Initialize packet content
    memset((void*)(this->content), 0, PACKET_SIZE + 1);
    // Set packet header
    header->encode(this->content);
    // Set packet message
    strncpy(this->content + HEADER_SIZE, message, _length);
}

void Packet::extract(char* buffer) {
    strncpy(buffer, this->content + HEADER_SIZE, _length);
}