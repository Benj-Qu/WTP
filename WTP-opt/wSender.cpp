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
    std::ifstream ifp(info.iFile);
    std::ofstream log(info.log);
    // Set Buffer And Seed
    char buffer[PACKET_SIZE + 1];
    srand((unsigned int)time(NULL));
    unsigned int seed = rand();

    // Send Start Packet
    Packet start(START, seed);
    start.sendPack(&sender, log);
    while (true) {
        if (recver.recv(buffer, PACKET_SIZE) <= 0) {
            start.sendPack(&sender, log);
            continue;
        }
        Packet packet(buffer, log);
        if (packet.header.type == ACK && packet.header.seqNum == seed) break;
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
            ifp.read(buffer, CHUNK_SIZE);
            // Create Packet
            unsigned int dsize = (unsigned int)ifp.gcount();
            Packet packet(DATA, seqNum++, dsize, buffer);
            // Send the packet
            packet.sendPack(&sender, log);
            window.push(packet);
        }
        // Receive Acks
        if (recver.recv(buffer, PACKET_SIZE) > 0) {
            Packet packet(buffer, log);
            if (packet.checkSum() && packet.header.type == ACK) {
                window.ack(packet.header.seqNum);
                window.senderForward();
            }
        }
        else {
            window.resendAll(&sender, log);
        }
    }

    // Send End Packet
    Packet end(END, seed);
    end.sendPack(&sender, log);
    while (true) {
        if (recver.recv(buffer, PACKET_SIZE) <= 0) {
            end.sendPack(&sender, log);
            continue;
        }
        Packet packet(buffer, log);
        if (packet.header.type == ACK && packet.header.seqNum == seed) break;
    }

    // Close Files And Socket
    ifp.close();
    log.close();
    close(sockfd);
    return 0;
}