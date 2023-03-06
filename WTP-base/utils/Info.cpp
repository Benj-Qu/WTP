#include "Info.hpp"

#include <iostream>

SenderInfo::SenderInfo(int argc, char** argv) {
    if (argc != 6) {
        std::cerr << "Usage: ./wSender <receiver-IP> <receiver-port> <window-size> <input-file> <log>" << std::endl;
        exit(1);
    }
    this->IP = argv[1];
    this->port = atoi(argv[2]);
    this->size = atoi(argv[3]);
    this->iFile = argv[4];
    this->log = argv[5];
}

ReceiverInfo::ReceiverInfo(int argc, char** argv) {
    if (argc != 5) {
        std::cerr << "Usage: ./wReceiver <port-num> <window-size> <output-dir> <log>" << std::endl;
        exit(1);
    }
    this->port = atoi(argv[1]);
    this->size = atoi(argv[2]);
    this->oDir = argv[3];
    this->log = argv[4];
}