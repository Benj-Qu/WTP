#include <iostream>

#include "utils/info.hpp"
#include "utils/clock.hpp"

#define INIT_SEQ 0
#define CHUNK_SIZE 1456

int main(int argc, char **argv) {
    SenderInfo info;
    parse_SenderInfo(&info, argc, argv);
    std::cout << info.IP << " " << info.port << " " << info.size << " " << info.iFile << " " << info.log << std::endl;

    Clock timer;
    for (int i = 0; i < 3; i++) {
        timer.reset();
        while (!timer.exceed());
        std::cout << "Loop " << i << std::endl;
    }

    return 0;
}
