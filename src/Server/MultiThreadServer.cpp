#include <MultiThreadServer.hpp>

namespace srv {
    MultiThreadServer::MultiThreadServer(const char* service) : 
        AbstractServer(service) {}
    MultiThreadServer::MultiThreadServer(const char* service, const int log) : 
        AbstractServer(service, log) {}
    void MultiThreadServer::mainloop() {
        struct sockaddr_in sin;     // the src address of a client
        unsigned int alen;          // length of client's address
        int msock;                  // master server socket
        int ssock;                  // slave server socket

        msock = cnt::passiveTCP(this->service, this->QLEN, this->log);

        while (1) {
            alen = sizeof(sin);
            ssock = accept(msock, (struct sockaddr *)&sin, &alen);
            if (ssock < 0) {
                cnt::errexit("accept: %s\n", strerror(errno));
            }
        }
    }
}
