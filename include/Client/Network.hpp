/*
 * Author: Ernie Chu
 * Filename: Network.hpp
 * Description: A back-end network ultility collection for the client
 */

#pragma once
#include <ConnectTCP.hpp>
#include <MessageWrapper.hpp>
#include <string>
#include <cstring>
#include <unistd.h>

namespace cli {
    class Network {
    public:       
        ~Network();
        std::string connect();
        void send(
            const std::string& receiver,
            const std::string& message,
            const int noSendBack=0
        );
        void sendUsage();
        proto::MessageWrapper receive();

        void setHost(std::string host);
        void setService(std::string service);
        void setUname(std::string uname);
    private:
        int fd;
        std::string host;
        std::string service;
        std::string uname;
    };
}
