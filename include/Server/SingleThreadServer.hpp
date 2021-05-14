/*
 * Author: Ernie Chu
 * Filename: SingleThreadServer.hpp
 * Description: A TCP server with single thread using select()
 */

#include <AbstractServer.hpp>
#include <sys/select.h>

namespace srv {
    class SingleThreadServer : public AbstractServer {
    public:
        SingleThreadServer() = delete;
        SingleThreadServer(const char* service);
        SingleThreadServer(const char* service, const int log);
        void mainloop();
    protected:
        void addNewClient(
            const struct sockaddr_in& sin,
            const int& fd
        );
        void readMessage(const int& fd);
        void writeMessage(const int& fd);
    };
}

