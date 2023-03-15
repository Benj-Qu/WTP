#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>

#include "utils/Info.hpp"
#include "utils/Packet.hpp"
#include "utils/PacketHeader.hpp"
#include "utils/Window.hpp"
#include "utils/udp.hpp"

int main(int argc, char **argv) {
    ReceiverInfo info(argc, argv);

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
    window.fill();
    // Create Directory
    std::string cmd = "mkdir " + info.oDir;
    struct stat st;
    if (stat(info.oDir.c_str(), &st) != 0) {
        if (system(cmd.c_str()) < 0) {
            std::cerr << "Failed Creating Directory" << std::endl;
            exit(1);
        }
    }
    else {
        std::cerr << "Directory Already Existed" << std::endl;
    }
    // Open Log File
    std::ofstream log(info.log);
    // Set Buffer
    char hdBuffer[HEADER_SIZE + 1];
    char buffer[PACKET_SIZE + 1];
    unsigned int seed = 0;

    // Keep Listening
    unsigned int index = 0;
    while(true) {
        // Receive START
        if (sender.recv(hdBuffer, HEADER_SIZE) == HEADER_SIZE) {
            PacketHeader start(hdBuffer, log);
            if (start.isSignal() && start.type == START) {
                seed = start.seqNum;
                start.type = ACK;
                start.sendHeader(&sender, log);
            }
            else if (start.isSignal() && start.type == END && start.seqNum == seed) {
                start.type = ACK;
                start.sendHeader(&sender, log);
                window.reset();
                continue;
            }
            else continue;
        }
        else continue;

        // Open Output File
        std::ostringstream oss;
        oss << info.oDir << "/FILE-" << index++ << ".out";
        std::string ofile = oss.str();
        std::ofstream ofp(ofile, std::ios::out | std::ios::binary);

        // Receive Data
        while (true) {
            if (sender.recv(buffer, PACKET_SIZE) > 0) {
                PacketHeader header(buffer, log);
                if (header.isSignal()) {
                    if (header.type == END && header.seqNum == seed) {
                        header.type = ACK;
                        header.sendHeader(&sender, log);
                        window.reset();
                        break;
                    }
                    else if (header.type == START && header.seqNum == seed) {
                        header.type = ACK;
                        header.sendHeader(&sender, log);
                    }
                }
                else if ((header.type == DATA) && (crc32(buffer + HEADER_SIZE, header.length) == header.checksum)) {
                    window.receive(header, buffer + HEADER_SIZE);
                    window.recverForward(ofp);
                    window.sendAck(&sender, log);
                }
            }
        }

        // Close File
        ofp.close();
    }

    log.close();
    close(sockfd);
    return 0;
}