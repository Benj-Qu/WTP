#include "PacketHeader.hpp"

#include <stdio.h>
#include <cstring>

#define INT_SIZE 4

PacketHeader::PacketHeader(char* buffer) {
    memcpy((void*)(&(this->type)), (void*)(buffer + 0 * INT_SIZE), INT_SIZE);
	memcpy((void*)(&(this->seqNum)), (void*)(buffer + 1 * INT_SIZE), INT_SIZE);
	memcpy((void*)(&(this->length)), (void*)(buffer + 2 * INT_SIZE), INT_SIZE);
	memcpy((void*)(&(this->checksum)), (void*)(buffer + 3 * INT_SIZE), INT_SIZE);
}

void PacketHeader::encode(char* buffer) {
    memcpy((void*)(buffer + 0 * INT_SIZE), (void*)(&(this->type)), INT_SIZE);
	memcpy((void*)(buffer + 1 * INT_SIZE), (void*)(&(this->seqNum)), INT_SIZE);
	memcpy((void*)(buffer + 2 * INT_SIZE), (void*)(&(this->length)), INT_SIZE);
	memcpy((void*)(buffer + 3 * INT_SIZE), (void*)(&(this->checksum)), INT_SIZE);
}

void PacketHeader::log(std::ofstream& ofs) {
	ofs << this->type << " " << this->seqNum << " " << this->length << " " << this->checksum << std::endl;
}