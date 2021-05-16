/*
 * Author: Ernie Chu
 * Filename: AbstractServer.cpp
 * Description: An abstract TCP server base class
 */

#include <AbstractServer.hpp>

namespace srv {
    AbstractServer::AbstractServer(const char* service) :
        service(service),
        log(0) {}
    AbstractServer::AbstractServer(const char* service, const int log) :
        service(service),
        log(log) {}

    std::string AbstractServer::makeOnlineMsg(
        const struct sockaddr_in& sin,
        const std::string& uname
    ) {
        return "User " + uname + " is on-line @" + std::string(inet_ntoa(sin.sin_addr)) + ".";
    }

    std::string AbstractServer::makeOfflineMsg(const std::string& uname) {
        return "User " + uname + " is off-line.";
    }

    void AbstractServer::messageLog(const proto::MessageWrapper& msg, const int recv) {
        std::cout << ((recv)? "Received message:\n": "Sent message:\n")
                  << ((recv)? "\tSender: ": "\tReceiver") << msg.uname << std::endl
                  << "\tMessage: " << msg.message << std::endl
                  << "\tTime: " << msg.timestamp << std::endl;
    }

    void AbstractServer::broadcast(const proto::MessageWrapper& msg) {
        // c++17
        for (auto const& [_, receiver] : this->onlineUsers)
            messagePool[receiver].push(msg); 
    }
    std::string AbstractServer::systemResponse(const std::string& cmd) {
        if (cmd == "list") {
            return "TODO: list all user";
        }
        else if (cmd == "logout")
            return "Bye!";
        else
            return cmd + ": command not found";
    }
}

