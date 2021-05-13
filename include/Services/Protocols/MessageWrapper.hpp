#pragma once
#include <ctime>

namespace proto {
    struct MessageWrapper {
        char uname[64];         // Used as sender when receiving; receiver when sending
        char message[512];
        std::time_t timestamp;
    };
}

