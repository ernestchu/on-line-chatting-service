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

        // make the socket non-blocking
        int flags;
        if ((flags = fcntl(msock, F_GETFL, 0)) == -1)
            flags = 0;
        fcntl(msock, F_SETFL, flags | O_NONBLOCK);

        while (1) {
            // clean the completed thread
            for (auto it = promises.begin(); it != promises.end(); ) {
                if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    it->get();
                    it = promises.erase(it);
                }
                it++;
            }
            
            // accept new connection (non-blocking)
            alen = sizeof(sin);
            ssock = accept(msock, (struct sockaddr *)&sin, &alen);
            if (ssock < 0) {
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                    continue;
                else
                    cnt::errexit("accept: %s\n", strerror(errno));
            }
            this->addNewClient(sin, ssock);

            // make a new thread to handle new client
            promises.emplace_back(
                std::async(std::launch::async, [&ssock, this] {
                    this->connectionHandler(ssock);
                })
            );
        }
    }
    void MultiThreadServer::connectionHandler(const int& fd) {
        auto readPromise = std::async(std::launch::async, [&fd, this] {
            while(this->readMessage(fd));
        });
        while (1) {
            this->writeMessage(fd);
            if (readPromise.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                readPromise.get();
                return;
            }
        }
    }
    void MultiThreadServer::removeClient(const int& fd) {
        // A user just went off-line
        this->mu.lock();
        auto sender = onlineUsers[fd];
        this->onlineUsers.erase(fd);
        this->mu.unlock();

        // send the offline message to all
        proto::MessageWrapper offBuf;
        std::strcpy(offBuf.uname, "System");
        std::strcpy(offBuf.message, this->makeOfflineMsg(sender).c_str());
        offBuf.timestamp = std::time(nullptr);

        this->broadcast(offBuf);

        if (this->log)
            std::cout << offBuf.message << std::endl;
    }
}
