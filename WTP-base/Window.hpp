#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <deque>

class Window {
private:
    unsigned int size;
    unsigned int head;
    std::deque<char*> ;

public:
    Window(usigned int _size) : size(_size) {};
};

#endif