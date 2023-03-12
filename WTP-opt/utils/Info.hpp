#ifndef __INFO_H__
#define __INFO_H__

#include <string>

class SenderInfo {
public:
    std::string IP;
    unsigned int port;
    unsigned int size;
    std::string iFile;
    std::string log;
    SenderInfo(int argc, char** argv);
};

struct ReceiverInfo {
public:
    unsigned int port;
    unsigned int size;
    std::string oDir;
    std::string log;
    ReceiverInfo(int argc, char** argv);
};

#endif