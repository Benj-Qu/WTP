#ifndef __UDP_H__
#define __UDP_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define CHUNK_SIZE 1456

int main(void)
{
    struct sockaddr_in addr;
    
    socklen_t slen = sizeof(addr);
    char buf[CHUNK_SIZE];
    char message[CHUNK_SIZE];
    sprintf(message, "Hello World!");

    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1)
    {
        die("socket");
    }

    memset((char *) &addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    struct hostent* sp = gethostbyname(SERVER);
    memcpy(&addr.sin_addr, sp->h_addr, sp->h_length);

    //send the message
    if (sendto(sockfd, message, strlen(message) , 0 , (struct sockaddr *) &addr, slen)==-1)
    {
        die("sendto()");
    }

    //receive a reply and print it
    //clear the buffer by filling null, it might have previously received data
    memset(buf,'\0', CHUNK_SIZE);
    //try to receive some data, this is a blocking call
    if (recvfrom(sockfd, buf, CHUNK_SIZE, 0, (struct sockaddr *) &addr, &slen) == -1)
    {
        die("recvfrom()");
    }
    printf("Received packet from %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    printf("Data: %s\n" , buf);

    close(sockfd);
    return 0;
}

#endif