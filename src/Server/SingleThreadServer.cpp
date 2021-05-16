#include <SingleThreadServer.hpp>

namespace srv {
    SingleThreadServer::SingleThreadServer(const char* service) : 
        AbstractServer(service) {}
    SingleThreadServer::SingleThreadServer(const char* service, const int log) : 
        AbstractServer(service, log) {}
    void SingleThreadServer::mainloop() {
        struct sockaddr_in sin;     // the src address of a client
        int msock;                  // master server socket
        fd_set rfds;                // read file descriptor set
        fd_set wfds;                // write file descriptor set
        fd_set afds;                // active file descriptor set
        unsigned int alen;          // from-address length
        int nfds=getdtablesize();   // number of file descriptors
        // nfds might be larger than FD_SETSIZE
        // e.g. on my macOS 11, but not on Ubuntu 20.04
        if (nfds > FD_SETSIZE) nfds = FD_SETSIZE;

        msock = cnt::passiveTCP(this->service, this->QLEN, this->log);

        FD_ZERO(&afds);
        FD_SET(msock, &afds);

        while(1) {
            memcpy(&rfds, &afds, sizeof(rfds)); // copy afds to rfds
            memcpy(&wfds, &afds, sizeof(wfds)); // copy afds to wfds
            
            // Select a subset from the active fds, they may be clients that
            //      1) ready to be read from (client that is writing to the server)
            //      2) ready to be written to (client that is reading from the server)
            // So the server would not perform blocking reading/writing call
            struct timeval zero = {0, 0};
            if (select(nfds, &rfds, &wfds, (fd_set*)0, &zero) < 0)
                cnt::errexit("select error: %s\n", strerror(errno));

            // Master socket: accept new request
            if (FD_ISSET(msock, &rfds)) {
                int ssock;
                alen = sizeof(sin);
                ssock = accept(msock, (struct sockaddr*)&sin, &alen);
                if (ssock < 0)
                    cnt::errexit("accept: error %s\n", strerror(errno));
                FD_SET(ssock, &afds);
                this->addNewClient(sin, ssock);
            }

            // Slave sockets: handle requests
            for (int fd = 0; fd < nfds; fd++) {
                // Ready to be read from
                if (fd != msock && FD_ISSET(fd, &rfds))
                    this->readMessage(fd, afds);
                // Ready to be written to
                else if (fd != msock && FD_ISSET(fd, &wfds))
                    this->writeMessage(fd);
            }
        }
    }
}
