#include <iostream>
#include "info.h"

int main(int argc, char **argv) {
    SenderInfo info;
    parse_SenderInfo(&info, argc, argv);

    std::cout << info.IP << info.port << info.size << info.iFile << info.log << std::endl;
    return 0;
}
