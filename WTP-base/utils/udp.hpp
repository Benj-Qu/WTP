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

    AddrInfo(int _sockfd, struct sockaddr* _addr, socklen_t _addrlen) :
        sockfd(_sockfd), addr(_addr), addrlen(_addrlen) {};
    int send(char* data, int len);
    int recv(char* buffer, int len);
};

#endif