/*
 * Author: Ernie Chu
 * Filename: PassiveUDP.hpp
 * Description: Create a passive socket for use in a UDP server
 * [Arguments]
 *  service:    Service associated with the desired port, also can be port number
 */

#pragma once
#include <PassiveSock.hpp>

namespace cnt {
    int passiveUDP(
        const char* service,
        int log=0
    );
}

