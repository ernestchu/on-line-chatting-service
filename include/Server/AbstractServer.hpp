/*
 * Author: Ernie Chu
 * Filename: AbstractServer.hpp
 * Description: An abstract TCP server base class
 */

#include <PassiveTCP.hpp>
#include <MessageWrapper.hpp>

#include <unistd.h>
#include <memory>
#include <unordered_set>
#include <queue>
#include <unordered_map>

namespace srv {
    class AbstractServer {
    public:
        AbstractServer() = delete;
        AbstractServer(const char* service);
        AbstractServer(const char* service, const int log);
        virtual void mainloop() = 0;
    protected:
        std::string makeOnlineMsg(
            const struct sockaddr_in& sin,
            const std::string& uname
        );
        std::string makeOfflineMsg(const std::string& uname);
        void messageLog(const proto::MessageWrapper& msg, const int recv=1);
        void broadcast(const proto::MessageWrapper& msg);

        const unsigned short QLEN    = 32;      // TCP queue length
        const unsigned short BUFSIZE =          // read/write buffer size
            sizeof(proto::MessageWrapper);
        const char* service;                    // service name, port number
        const int log;

        std::unordered_map<                     // Online users (lookup by fd)
            int,
            std::string
        > onlineUsers;

        std::unordered_map<                     // A pool for all of the unsent messages
            std::string,                        // (lookup by username)
            std::queue<proto::MessageWrapper>
        > messagePool;

        std::unordered_set<                     // Registered users in the history
            std::string
       > registeredUsers;
    };
}

