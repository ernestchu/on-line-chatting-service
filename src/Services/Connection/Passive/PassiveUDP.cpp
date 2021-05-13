/*
 * Author: Ernie Chu
 * Filename: PassiveUDP.cpp
 * Description: Create a passive socket for use in a UDP server
 * [Arguments]
 *  service:    Service associated with the desired port, also can be port number
 */

#include <PassiveUDP.hpp>

namespace cnt {
    int passiveUDP(
        const char* service,
        int log
    ) {
        return passiveSock(service, "udp", 0, log);
    }
}

