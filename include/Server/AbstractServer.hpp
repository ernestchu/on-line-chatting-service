/*
 * Author: Ernie Chu
 * Filename: AbstractServer.hpp
 * Description: An abstract TCP server base class
 */

#pragma once
#include <PassiveTCP.hpp>
#include <MessageWrapper.hpp>

#include <unistd.h>
#include <memory>
#include <algorithm> // set_difference
#include <set>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include <cstring>
#include <mutex>
#include <openabe/openabe.h>

namespace srv {
    class AbstractServer {
    public:
        AbstractServer() = delete;
        AbstractServer(const char* service);
        AbstractServer(const char* service, const int log);
        virtual ~AbstractServer() = default;
        virtual void mainloop() = 0;
    protected:
        // function
        void            addNewClient(const struct sockaddr_in& sin, const int& fd);
        virtual void    removeClient(const int& fd) = 0;
        int             readMessage(const int& fd);
        void            writeMessage(const int& fd);

        std::string     makeOnlineMsg(const struct sockaddr_in& sin, const std::string& uname);
        std::string     makeOfflineMsg(const std::string& uname);
        void            messageLog(const proto::MessageWrapper& msg, const int recv=1);
        void            broadcast(const proto::MessageWrapper& msg);
        std::string     systemResponse(const std::string& cmd);
        std::string     dumpUsers();

        // variable
        const unsigned short QLEN    = 32;                              // TCP queue length
        const unsigned short BUFSIZE = sizeof(proto::MessageWrapper);   // read/write buffer size
        const char* service;                                            // service name, port number
        const int log;
        std::mutex mu;
        std::string mpk;    // ABE master public key
        std::string msk;    // ABE master private (secret) key

        std::unordered_map<                                             // Online users (lookup by fd)
            int,
            std::string
        > onlineUsers;

        std::unordered_map<                                             // A pool for all of the unsent messages
            std::string,                                                // (lookup by username)
            std::queue<proto::MessageWrapper>
        > messagePool;

        std::unordered_set<                                             // Registered users in the history
            std::string
        > registeredUsers;
    };
}

