#include <iostream>

#include "Info.hpp"

void parse_SenderInfo(SenderInfo* info, int argc, char** argv) {
    if (argc != 6) {
        std::cerr << "Usage: ./wSender <receiver-IP> <receiver-port> <window-size> <input-file> <log>" << std::endl;
        exit(1);
    }
    info->IP = argv[1];
    info->port = atoi(argv[2]);
    info->size = atoi(argv[3]);
    info->iFile = argv[4];
    info->log = argv[5];
}

void parse_ReceiverInfo(ReceiverInfo* info, int argc, char** argv) {
    if (argc != 5) {
        std::cerr << "Usage: ./wReceiver <port-num> <window-size> <output-dir> <log>" << std::endl;
        exit(1);
    }
    info->port = atoi(argv[1]);
    info->size = atoi(argv[2]);
    info->oDir = argv[3];
    info->log = argv[4];
}