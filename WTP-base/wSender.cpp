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

std::mutex mtx;
std::condition_variable cv;

Window window;
Clock timer;
bool TimeOut = false;

void CheckTime();
void CheckAck();

/*  Thread 0: 
 *  Build Connection. Send START Packet. Send File As Packets. Send END Packet.
 *  Push Packet To Window When Sent.
 *  Blocked by CV When Window Full.
 */
int main(int argc, char **argv) {
    SenderInfo info(argc, argv);

    // TODO: Set Socket

    // Create Sender Window
    window = Window(info.size);

    // Open Input File and Log File
    std::ifstream ifp(info.iFile);
    std::ofstream ofp("test.out");
    std::ofstream log(info.log);

    // TODO: Send Start Packet
    char buffer[CHUNK_SIZE + 1];
    int seed = rand();
    memset(buffer, 0, sizeof(buffer));
    Packet start(START, seed, 0, buffer);
    start.header.log(log);

    // TODO: Wait Until ACK For Start

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
        // TODO: Send the packet
        ofp.write(buffer, dsize);
        // Log Sending Activity
        packet.header.log(log);
    }

    for (int i = 0; i < 3; i++) {
        timer.reset();
        while (!timer.exceed());
        std::cout << "Loop " << i << std::endl;
    }

    // TODO: Send End Packet
    memset(buffer, 0, sizeof(buffer));
    Packet end(END, seed, 0, buffer);
    end.header.log(log);

    // TODO: Wait Until ACK For End

    // Close Files
    ifp.close();
    ofp.close();
    log.close();
    return 0;
}

/*  Thread 1:
 *  Check TimeOut. 
 *  When TimeOut, Resend Everything In Window.
 */
void CheckTime() {

}

/*  Thread 2:
 *  Check ACK. 
 *  When ACK Received, Forward (Pop) Window And Notify CV.
 */
Void CheckAck() {
    
}