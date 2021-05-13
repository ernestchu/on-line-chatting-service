/*
 * Author: Ernie Chu
 * Filename: AbstractServer.cpp
 * Description: An abstract TCP server base class
 */

#include <AbstractServer.hpp>

namespace srv {
    AbstractServer::AbstractServer(const char* service) :
        service(service)
    {}
    
    std::string AbstractServer::makeOfflineMsg(std::string uname) {
        return "User " + uname + " is off-line.";
    }
}

