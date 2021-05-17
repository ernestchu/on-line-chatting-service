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

        std::vector<std::future<void>> promises; // vector of thread's promise

        msock = cnt::passiveTCP(this->service, this->QLEN, this->log);

        while (1) {
            alen = sizeof(sin);
            ssock = accept(msock, (struct sockaddr *)&sin, &alen);
            if (ssock < 0)
                cnt::errexit("accept: %s\n", strerror(errno));

            // make a new thread to handle new client
            promises.emplace_back(
                std::async(std::launch::async, [&ssock, this] {
                    this->connectionHandler(ssock);
                })
            );

            // clean the completed thread
            for (auto it = promises.begin(); it != promises.end(); ) {
                if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    it->get();
                    it = promises.erase(it);
                }
                it++;
            }
        }
    }
    void MultiThreadServer::connectionHandler(int fd) {

    }
}
