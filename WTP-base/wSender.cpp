#include <iostream>
#include <fstream>
#include <cstring>

#include "utils/Info.hpp"
#include "utils/Packet.hpp"
#include "utils/PacketHeader.hpp"
#include "utils/Window.hpp"
#include "utils/udp.hpp"

#define RETRANS_TIME 500

int main(int argc, char **argv) {
    SenderInfo info(argc, argv);

    // Set Socket
    struct sockaddr_in sendaddr, recvaddr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        std::cerr << "Failed Creating Socket" << std::endl;
        exit(1);
    }
    // Set Timeout
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000 * RETRANS_TIME;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    // Set Address
    memset(&sendaddr, 0, sizeof(sendaddr));
    sendaddr.sin_family = AF_INET;
    sendaddr.sin_port = htons((uint16_t)info.port);
    struct hostent* sp = gethostbyname(info.IP.c_str());
    memcpy(&sendaddr.sin_addr, sp->h_addr, sp->h_length);
    AddrInfo sender(sockfd, (struct sockaddr*)&sendaddr, sizeof(sendaddr));
    AddrInfo recver(sockfd, (struct sockaddr*)&recvaddr, sizeof(recvaddr));

    // Create Sender Window
    Window window = Window(info.size);
    // Open Input File and Log File
    std::ifstream ifp(info.iFile, std::ios::in | std::ios::binary);
    std::ofstream log(info.log);
    // Set Buffer And Seed
    char buffer[HEADER_SIZE + 1];
    srand((unsigned int)time(NULL));
    unsigned int seed = rand();

    // Send Start Packet
    PacketHeader start(START, seed);
    start.sendHeader(&sender, log);
    while (true) {
        if (recver.recv(buffer, HEADER_SIZE) != HEADER_SIZE) {
            start.sendHeader(&sender, log);
            continue;
        }
        PacketHeader ack(buffer, log);
        if (ack.type == ACK && ack.seqNum == seed && ack.isSignal()) break;
    }

    // Send Packets
    unsigned int seqNum = INIT_SEQ;
    if (!ifp) {
        std::cerr << "Imput File Empty" << std::endl;
        exit(1);
    }
    while (ifp || !window.empty()) {
        // Send Packets
        while (ifp && !window.full()) {
            // Read File
            window.read(ifp, seqNum++);
            // Send the packet
            window.sendback(&sender, log);
        }
        // Receive Acks
        if (recver.recv(buffer, HEADER_SIZE) == HEADER_SIZE) {
            PacketHeader ack(buffer, log);
            if (ack.type == ACK && ack.isSignal()) {
                window.cumulForward(ack.seqNum);
            }
        }
        else {
            window.sendall(&sender, log);
        }
    }

    // Send End Packet
    PacketHeader end(END, seed);
    end.sendHeader(&sender, log);
    while (true) {
        if (recver.recv(buffer, HEADER_SIZE) != HEADER_SIZE) {
            end.sendHeader(&sender, log);
            continue;
        }
        PacketHeader ack(buffer, log);
        if (ack.type == ACK && ack.seqNum == seed && ack.isSignal()) break;
    }

    // Close Files And Socket
    ifp.close();
    log.close();
    close(sockfd);
    return 0;
}