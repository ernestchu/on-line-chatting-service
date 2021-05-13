/*
 * Author: Ernie Chu
 * Filename: PassiveTCP.hpp
 * Description: Connect to a passive socket for use in a TCP server
 * [Arguments]
 *  service:    Service associated with the desired port, also can be port number
 *  qlen:       Maximum server request queue length
 */

#pragma once
#include <PassiveSock.hpp>

namespace cnt {
    int passiveTCP(
        const char* service,
        int qlen,
        int log=0
    );
}

