/*
 * Author: Ernie Chu
 * Filename: MultiThreadServer.hpp
 * Description: A TCP server with multiple threads
 */

#pragma once
#include <AbstractServer.hpp>

namespace srv {
    class MultiThreadServer : public AbstractServer {
    public:
        MultiThreadServer() = delete;
        MultiThreadServer(const char* service);
        MultiThreadServer(const char* service, const int log);
        void mainloop();
    };
}

