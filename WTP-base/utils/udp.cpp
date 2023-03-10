#include "udp.hpp"

#include <string.h>

ssize_t AddrInfo::send(char* data, size_t len) {
    return sendto(this->sockfd, data, len, 0, this->addr, this->addrlen);
}

ssize_t AddrInfo::recv(char* buffer, size_t len) {
    return recvfrom(this->sockfd, buffer, len, 0, this->addr, &(this->addrlen));
}