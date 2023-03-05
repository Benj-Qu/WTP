#ifndef __INFO_H__
#define __INFO_H__

#include <string>

struct SenderInfo
{
    std::string IP;
    unsigned int port;
    unsigned int size;
    std::string iFile;
    std::string log;
};

struct ReceiverInfo
{
    unsigned int port;
    unsigned int size;
    std::string oDir;
    std::string log;
};

void parse_SenderInfo(SenderInfo* info, int argc, char** argv);
void parse_ReceiverInfo(ReceiverInfo* info, int argc, char** argv);

#endif