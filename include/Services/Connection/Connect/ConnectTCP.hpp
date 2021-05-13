/*
 * Author: Ernie Chu
 * Filename: ConnectTCP.hpp
 * Description: Connect to a TCP service on a host
 * [Arguments]
 *  host:       Name of host to which connection is desired, also can be IP address
 *  service:    Service associated with the desired port, also can be port number
 */

#pragma once
#include <ConnectSock.hpp>

namespace cnt {
    int connectTCP(
        const char* host,
        const char* service
    );
}

