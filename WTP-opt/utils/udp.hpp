#ifndef __UDP_H__
#define __UDP_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

class AddrInfo {
public:
    int sockfd;
    struct sockaddr* addr;
    socklen_t addrlen;

    AddrInfo(int _sockfd) : sockfd(_sockfd) {};
    AddrInfo(int _sockfd, struct sockaddr* _addr, socklen_t _addrlen) :
        sockfd(_sockfd), addr(_addr), addrlen(_addrlen) {};
    ssize_t send(char* data, size_t len);
    ssize_t recv(char* buffer, size_t len);
};

#endif