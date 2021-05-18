#include <SingleThreadServer.hpp>

namespace srv {
    SingleThreadServer::SingleThreadServer(const char* service) : 
        AbstractServer(service) {
            FD_ZERO(&this->afds);
        }
    SingleThreadServer::SingleThreadServer(const char* service, const int log) : 
        AbstractServer(service, log) {
            FD_ZERO(&this->afds);
        }
    void SingleThreadServer::mainloop() {
        struct sockaddr_in sin;     // the src address of a client
        int msock;                  // master server socket
        fd_set rfds;                // read file descriptor set
        fd_set wfds;                // write file descriptor set
        unsigned int alen;          // from-address length
        int nfds=getdtablesize();   // number of file descriptors
        // nfds might be larger than FD_SETSIZE
        // e.g. on my macOS 11, but not on Ubuntu 20.04
        if (nfds > FD_SETSIZE) nfds = FD_SETSIZE;

        msock = cnt::passiveTCP(this->service, this->QLEN, this->log);

        FD_ZERO(&this->afds);
        FD_SET(msock, &this->afds);

        while(1) {
            memcpy(&rfds, &this->afds, sizeof(rfds)); // copy afds to rfds
            memcpy(&wfds, &this->afds, sizeof(wfds)); // copy afds to wfds
            
            // Select a subset from the active fds, they may be clients that
            //      1) ready to be read from (client that is writing to the server)
            //      2) ready to be written to (client that is reading from the server)
            // So the server would not perform blocking reading/writing call
            if (select(nfds, &rfds, &wfds, (fd_set*)0, (struct timeval*)0) < 0)
                cnt::errexit("select error: %s\n", strerror(errno));

            // Master socket: accept new request
            if (FD_ISSET(msock, &rfds)) {
                int ssock;
                alen = sizeof(sin);
                ssock = accept(msock, (struct sockaddr*)&sin, &alen);
                if (ssock < 0)
                    cnt::errexit("accept: error %s\n", strerror(errno));
                FD_SET(ssock, &this->afds);
                this->addNewClient(sin, ssock);
            }

            // Slave sockets: handle requests
            for (int fd = 0; fd < nfds; fd++) {
                // Ready to be read from
                if (fd != msock && FD_ISSET(fd, &rfds))
                    this->readMessage(fd);
                // Ready to be written to
                else if (fd != msock && FD_ISSET(fd, &wfds))
                    this->writeMessage(fd);
            }
        }
    }
    void SingleThreadServer::removeClient(const int& fd) {
        // A user just went off-line
        this->mu.lock();
        FD_CLR(fd, &this->afds);
        auto sender = this->onlineUsers[fd];
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
