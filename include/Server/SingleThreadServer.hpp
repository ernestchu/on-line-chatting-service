/*
 * Author: Ernie Chu
 * Filename: SingleThreadServer.hpp
 * Description: A TCP server with single thread using select()
 */

#include <AbstractServer.hpp>

namespace srv {
    class SingleThreadServer : public AbstractServer {
    public:
        SingleThreadServer() = delete;
        SingleThreadServer(const char* service);
        void mainloop(int log=0);
    protected:
        void readMessage(int fd);
        void writeMessage(int fd);
    };
}

