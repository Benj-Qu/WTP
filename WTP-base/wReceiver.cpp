#include <iostream>
#include <fstream>
#include <cstring>
#include <filesystem>

#include "utils/Info.hpp"
#include "utils/Packet.hpp"
#include "utils/PacketHeader.hpp"
#include "utils/Window.hpp"
#include "utils/udp.hpp"

int main(int argc, char **argv) {
    ReceiverInfo info(argc, argv);
    std::cout << info.port << " " << info.size << " " << info.oDir << " " << info.log << std::endl;

    // Set Socket
    struct sockaddr_in sendaddr, recvaddr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        std::cerr << "Failed Creating Socket" << std::endl;
        exit(1);
    }
    // Set Address
    memset(&recvaddr, 0, sizeof(recvaddr));
    recvaddr.sin_family = AF_INET;
    recvaddr.sin_port = htons((uint16_t)info.port);
    recvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // Bind Socket
    if(bind(sockfd, (struct sockaddr*)&recvaddr, sizeof(recvaddr) ) == -1) {
        std::cerr << "Failed Binding Socket" << std::endl;
        exit(1);
    }
    AddrInfo sender(sockfd, (struct sockaddr*)&sendaddr, sizeof(sendaddr));
    AddrInfo recver(sockfd, (struct sockaddr*)&recvaddr, sizeof(recvaddr));

    // Create Sender Window
    Window window = Window(info.size);
    // Create Directory
    std::filesystem::create_directory(info.oDir);
    // Open Log File
    std::ofstream log(info.log);
    // Set Buffer
    char buffer[CHUNK_SIZE + 1];
    unsigned int seed = 0;

    // Keep Listening
    while(true) {
        // Receive START
        memset(buffer, 0, sizeof(buffer));
        if (sender.recv(buffer, PACKET_SIZE) > 0) {
            Packet packet(buffer, log);
            if (packet.header.type == START) {
                seed = packet.header.seqNum;
            }
            else continue;
        }

        
    }

    close(s);
    return 0;
}