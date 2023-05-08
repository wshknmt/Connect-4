#ifndef DEBUG_H
#define DEBUG_H

#ifndef NDEBUG
#include <iostream>

template <typename ... T>
void print(const T& ... t) {
    ((std::cout << t), ...);
    std::cout << std::endl;
}
#define DEBUG(...)

#else

#define DEBUG(...)

#endif


#endif // DEBUG_H
