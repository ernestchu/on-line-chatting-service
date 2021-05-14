/*
 * Author: Ernie Chu
 * Filename: ConnectSock.hpp
 * Description: Allocate and connect a socket using TCP or UDP
 * [Argument]
 *  host:       Name of host to which connection is desires, can also be IP address
 *  service:    Service associated with the desired port, can also be port number
 *  transport:  Name of transport protocol to use ("tcp" or "udp")
 */

#pragma once
#include <Errexit.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif

namespace cnt {
    int connectSock(
        const char* host,
        const char* service,
        const char* transport
    );
}

