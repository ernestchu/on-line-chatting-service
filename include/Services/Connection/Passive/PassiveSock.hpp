/*
 * Author: Ernie Chu
 * Filename: PassiveSock.hpp
 * Description: Allocate and bind a server socket using TCP or UDP
 * [Argument]
 *  service:    Service associated with the desired port, can also be port number
 *  transport:  Name of transport protocol to use ("tcp" or "udp")
 *  qlen:       Maximum server request queue length
 */

#pragma once
#include <Errexit.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <ifaddrs.h> // struct ifaddrs
#include <iostream> // for log == 1

extern int errno;

namespace cnt {
    int passiveSock(
        const char* service,
        const char* transport,
        int qlen,
        int log=0
    );
    void printHostAddrs(unsigned short port);
}

