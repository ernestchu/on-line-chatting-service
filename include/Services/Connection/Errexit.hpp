/*
 * Author: Ernie Chu
 * Filename: Errexit.hpp
 * Description: Print an error message and exit
 */

#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

namespace cnt {
    int errexit(const char* format, ...);
}

