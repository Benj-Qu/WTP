#include "udp.hpp"

#include <string.h>

int AddrInfo::send(char* data, int len) {
    return sendto(this->sockfd, data, len, 0, this->addr, this->addrlen);
}

int AddrInfo::recv(char* buffer, int len) {
    return recvfrom(this->sockfd, buffer, len, 0, this->addr, &(this->addrlen));
}