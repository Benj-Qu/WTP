#include <iostream>
#include <fstream>
#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "utils/Info.hpp"
#include "utils/Clock.hpp"
#include "utils/Packet.hpp"
#include "utils/PacketHeader.hpp"
#include "utils/Window.hpp"
#include "utils/udp.hpp"

std::mutex mtx;
std::mutex lk;
std::condition_variable cv;

Window window;
Clock timer;
bool alive = true;

void CheckTime(AddrInfo* sender, std::ofstream& log);
void CheckAck(AddrInfo* recver, std::ofstream& log);
void SendSignal(Packet packet, AddrInfo* sender, std::ofstream& log);

/*  Thread 0: 
 *  Build Connection. Send START Packet. Send File As Packets. Send END Packet.
 *  Push Packet To Window When Sent.
 *  Blocked by CV When Window Full.
 */
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
    window = Window(info.size);

    // Open Input File and Log File
    std::ifstream ifp(info.iFile);
    std::ofstream log(info.log);

    // Set Buffer And Seed
    char buffer[CHUNK_SIZE + 1];
    int seed = rand();

    // Start Threads
    std::thread tTimeout(CheckTime, &sender, log);
    std::thread tAck(CheckAck, &recver, log);

    // Send Start Packet
    memset(buffer, 0, sizeof(buffer));
    Packet start(START, seed, 0, buffer);
    SendSignal(start, &sender, log);

    // Reset Timer
    timer.reset();

    // Send Packets
    unsigned int seqNum = INIT_SEQ;
    while (ifp) {
        // Wait When Window Full
        std::unique_lock<std::mutex> lck(mtx);
        while (window.full()) cv.wait(lck);
        // Read File
        memset(buffer, 0, sizeof(buffer));
        ifp.read(buffer, CHUNK_SIZE);
        // Create Packet
        unsigned int dsize = (unsigned int)ifp.gcount();
        Packet packet(DATA, seqNum++, dsize, buffer);
        // Send the packet
        packet.sendPack(&sender, log);
        lk.lock();
        window.push(packet);
        lk.unlock();
        // Log Sending Activity
        packet.header.log(log);
    }

    // Send End Packet
    memset(buffer, 0, sizeof(buffer));
    Packet end(END, seed, 0, buffer);
    SendSignal(end, &sender, log);

    // Stop Threads
    alive = false;
    tTimeout.join();
    tAck.join();

    // Close Files
    ifp.close();
    log.close();
    return 0;
}

/*  Thread 1:
 *  Check TimeOut. 
 *  When TimeOut, Resend Everything In Window.
 */
void CheckTime(AddrInfo* sender, std::ofstream& log) {
    while (alive) {
        if (timer.exceed()) {
            // Resend Everything In Window
            lk.lock();
            window.sendall(sender, log);
            lk.unlock();
            // Reset Timer
            timer.reset();
        }
    }
}

/*  Thread 2:
 *  Check ACK. 
 *  When Proper ACK Received, Forward (Pop) Window And Notify CV.
 */
void CheckAck(AddrInfo* recver, std::ofstream& log) {
    char buffer[PACKET_SIZE + 1];
    while (alive) {
        // Listen ACK
        memset(buffer, 0, sizeof(buffer));
        if (recver->recv(buffer, PACKET_SIZE) <= 0) continue;
        // Decode Packet
        Packet packet(buffer, log);
        // Checksum
        if (!packet.checkSum()) continue;
        // Check Packet Type
        if (packet.header.type != ACK) continue;
        // Forward Window
        lk.lock();
        window.cumulForward(packet.header.seqNum);
        lk.unlock();
        cv.notify_one();
    }
}

// Send START And END Message
void SendSignal(Packet packet, AddrInfo* sender, std::ofstream& log) {
    // Reset Timer
    timer.reset();
    // Send Packet
    packet.sendPack(sender, log);
    lk.lock();
    window.push(packet);
    lk.unlock();
    // Block Until Window Empty
    std::unique_lock<std::mutex> lck(mtx);
    while (!window.empty()) cv.wait(lck);
}