/*
 * Author: Ernie Chu
 * Filename: ConnectTCP.cpp
 * Description: Connect to a TCP service on a host
 * [Arguments]
 *  host:       Name of host to which connection is desired, also can be IP address
 *  service:    Service associated with the desired port, also can be port number
 */


#include <ConnectTCP.hpp>

namespace cnt {
    int connectTCP(const char* host, const char* service) {
        return connectSock(host, service, "tcp");
    }
}

