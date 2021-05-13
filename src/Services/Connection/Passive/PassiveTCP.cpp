/*
 * Author: Ernie Chu
 * Filename: PassiveTCP.cpp
 * Description: Connect to a passive socket for use in a TCP server
 * [Arguments]
 *  service:    Service associated with the desired port, also can be port number
 *  qlen:       Maximum server request queue length
 */

#include <PassiveTCP.hpp>

namespace cnt {
    int passiveTCP(
        const char* service,
        int qlen,
        int log
    ) {
        return passiveSock(service, "tcp", qlen, log);
    }
}

