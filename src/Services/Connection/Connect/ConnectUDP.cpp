/*
 * Author: Ernie Chu
 * Filename: ConnectUDP.cpp
 * Description: Connect to a UDP service on a host
 * [Arguments]
 *  host:       Name of host to which connection is desired, also can be IP address
 *  service:    Service associated with the desired port, also can be port number
 */


#include <ConnectUDP.hpp>

namespace cnt {
    int connectUDP(const char* host, const char* service) {
        return connectSock(host, service, "udp");
    }
}

