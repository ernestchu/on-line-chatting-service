/*
 * Author: Ernie Chu
 * Filename: MessageWrapper.hpp
 * Description: A data structure to wrap up the related informations with messages
 */

#pragma once
#include <ctime>

namespace proto {
    struct MessageWrapper {
        char uname[64];         // Used as sender when receiving; receiver when sending
        char message[512];
        std::time_t timestamp;
        int noSendBack;           // if noSendBack==0, send the msg back to the sender
    };
}

