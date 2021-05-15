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
        static constexpr int inputLines = 4;
    };
}
