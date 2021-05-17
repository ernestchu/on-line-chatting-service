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
        fd_set rfds;                // read file descriptor set

        std::vector<std::future<void>> futures; // vector of thread's future

        msock = cnt::passiveTCP(this->service, this->QLEN, this->log);
        FD_ZERO(&rfds);
        FD_SET(msock, &rfds);
        this->mu.lock();
        std::cout << "Master thread ID: " << std::this_thread::get_id() << std::endl;
        this->mu.unlock();

        while (1) {
            if (select(msock+1, &rfds, (fd_set*)0, (fd_set*)0, (struct timeval*)0) < 0)
                cnt::errexit("select error: %s\n", strerror(errno));
            
            if (FD_ISSET(msock, &rfds)) {
                // accept new connection
                alen = sizeof(sin);
                ssock = accept(msock, (struct sockaddr *)&sin, &alen);
                if (ssock < 0) {
                        cnt::errexit("accept: %s\n", strerror(errno));
                }
                this->addNewClient(sin, ssock);

                // make a new thread to handle new client
                futures.emplace_back(
                    std::async(std::launch::async, [ssock, this] {
                        this->connectionHandler(ssock);
                    })
                );
            }
            // clean the completed thread
            for (auto it = futures.begin(); it != futures.end(); ) {
                if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    it->get();
                    it = futures.erase(it);
                } else
                    it++;
            }
        }
    }
    void MultiThreadServer::connectionHandler(const int& fd) {
        this->mu.lock();
        std::cout << "Slave thread ID: " << std::this_thread::get_id() << std::endl;
        this->mu.unlock();

        auto readFuture = std::async(std::launch::async, [&fd, this] {
            while(this->readMessage(fd));
        });
        while (1) {
            this->writeMessage(fd);
            if (readFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                readFuture.get();
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
