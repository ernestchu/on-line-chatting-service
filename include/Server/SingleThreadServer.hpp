/*
 * Author: Ernie Chu
 * Filename: SingleThreadServer.hpp
 * Description: A TCP server with single thread using select()
 */

#pragma once
#include <AbstractServer.hpp>

namespace srv {
    class SingleThreadServer : public AbstractServer {
    public:
        SingleThreadServer() = delete;
        SingleThreadServer(const char* service);
        SingleThreadServer(const char* service, const int log);
        void mainloop();
    private:
        void removeClient(const int& fd);
        fd_set afds;                // active file descriptor set
    };
}

