#include <iostream>
#include "utils/info.h"

int main(int argc, char **argv) {
    ReceiverInfo info;
    parse_ReceiverInfo(&info, argc, argv);

    std::cout << info.port << " " << info.size << " " << info.oDir << " " << info.log << std::endl;
    return 0;
}