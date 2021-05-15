#pragma once
#include <ConnectTCP.hpp>
#include <MessageWrapper.hpp>
#include <string>
#include <cstring>
#include <unistd.h>

namespace cli {
    class Network {
    public:       
        void connect();
        void sendMessage(std::string message);

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
