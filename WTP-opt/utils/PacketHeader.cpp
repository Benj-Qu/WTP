#include "PacketHeader.hpp"

#include <stdio.h>
#include <cstring>

#define INT_SIZE 4

PacketHeader::PacketHeader(char* buffer) {
    memcpy(&(this->type), buffer + 0 * INT_SIZE, INT_SIZE);
	memcpy(&(this->seqNum), buffer + 1 * INT_SIZE, INT_SIZE);
	memcpy(&(this->length), buffer + 2 * INT_SIZE, INT_SIZE);
	memcpy(&(this->checksum), buffer + 3 * INT_SIZE, INT_SIZE);
}

void PacketHeader::encode(char* buffer) {
    memcpy(buffer + 0 * INT_SIZE, &(this->type), INT_SIZE);
	memcpy(buffer + 1 * INT_SIZE, &(this->seqNum), INT_SIZE);
	memcpy(buffer + 2 * INT_SIZE, &(this->length), INT_SIZE);
	memcpy(buffer + 3 * INT_SIZE, &(this->checksum), INT_SIZE);
}

void PacketHeader::log(std::ofstream& log) {
	log << this->type << " " << this->seqNum << " " << this->length << " " << this->checksum << std::endl;
}