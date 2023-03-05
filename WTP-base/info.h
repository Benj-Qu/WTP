#ifndef __INFO_H__
#define __INFO_H__

#include <string>

using std::string;

struct SenderInfo
{
    string IP;
    unsigned int port;
    unsigned int size;
    string iFile;
    string log;
};

struct ReceiverInfo
{
    unsigned int port;
    unsigned int size;
    string oDir;
    string log;
};

void parse_SenderInfo(SenderInfo* info, int argc, char** argv);
void parse_ReceiverInfo(ReceiverInfo* info, int argc, char** argv);

#endif