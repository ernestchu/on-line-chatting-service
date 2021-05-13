/*
* Author: Ernie Chu
* Filename: Errexit.cpp
* Description: Print an error message and exit
*/

#include <Errexit.hpp>

namespace cnt {
    int errexit(const char* format, ...) {
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
        exit(1);
    }
}

