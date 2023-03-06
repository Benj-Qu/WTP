#include "Packet.hpp"

Packet::Packet(PacketHeader* header, char* message) {
    this->acked = false;
    // Initialize packet content
    memset((void*)(this->content), 0, PACKET_SIZE + 1);
    // Set packet header
    header->encode(this->content);
    // Set packet message
    strncpy(this->content + HEADER_SIZE, message, CHUNK_SIZE);
}

void Packet::extract(char* buffer) {
    strncpy(buffer, this->content + HEADER_SIZE, CHUNK_SIZE);
}