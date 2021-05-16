/*
 * Author: Ernie Chu
 * Filename: WindowConfig.hpp
 * Description: Configuration for all windows. Paddings, input window height, etc.
 */

#pragma once
#include <ncurses.h>
#include <string>
#include <cstring>
#include <cmath>
#include <MessageWrapper.hpp>
#include <algorithm> // count

namespace win {
    class WindowConfig {
    public:
        static constexpr double paddingRatio = .02;
        static constexpr int inputLines = 3;
    };
}
