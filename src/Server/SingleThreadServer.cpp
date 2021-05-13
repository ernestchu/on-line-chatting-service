/*
 * Author: Ernie Chu
 * Filename: SingleThreadServer.hpp
 * Description: A TCP server with single thread using select()
 */

#include <SingleThreadServer.hpp>

namespace srv {
    SingleThreadServer::SingleThreadServer(const char* service) : AbstractServer(service) {}
    void SingleThreadServer::mainloop(int log) {
        struct sockaddr_in fsin;    // the src address of a client
        int msock;                  // master server socket
        fd_set rfds;                // read file descriptor set
        fd_set wfds;                // write file descriptor set
        fd_set afds;                // active file descriptor set
        unsigned int alen;          // from-address length
        int nfds=getdtablesize();   // number of file descriptors

        msock = cnt::passiveTCP(this->service, this->QLEN, log);

        FD_ZERO(&afds);
        FD_SET(msock, &afds);

        while(1) {
            memcpy(&rfds, &afds, sizeof(rfds)); // copy afds to rfds
            memcpy(&wfds, &afds, sizeof(wfds)); // copy afds to wfds
            
            // Select a subset from the active fds, it may clients that
            //      1) ready to be read from (client that is writing to the server)
            //      2) ready to be written to (client that is reading from the server)
            // So the server would not perform blocking reading/writing call
            if (select(nfds, &rfds, &wfds, (fd_set*)0, (struct timeval*)0) < 0)
                cnt::errexit("select error: %s\n", strerror(errno));
        
            // Master socket: accept new request
            if (FD_ISSET(msock, &rfds)) {
                int ssock;
                alen = sizeof(fsin);
                ssock = accept(msock, (struct sockaddr*)&fsin, &alen);
                if (ssock < 0)
                    cnt::errexit("accept: error %s\n", strerror(errno));
                FD_SET(ssock, &afds);
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

    void SingleThreadServer::readMessage(int fd) {
        proto::MessageWrapper buf; // buf.uname is whom fd wants to send to

        int nbytes = read(fd, reinterpret_cast<char*>(&buf), sizeof(buf));

        if (nbytes < 0)
            cnt::errexit("Read message failed: %s\n", strerror(errno));
        else if (nbytes > 0) {
            // Normal message transmission

            std::string sender   = onlineUsers[fd];
            std::string receiver = buf.uname;
            std::strcpy(buf.uname, sender.c_str());

            if (this->registeredUsers.find(receiver) != this->registeredUsers.end())
                messagePool[receiver].push(buf);
            else
                // if the sender receives his own uname -> receiver not found!
                messagePool[sender].push(buf); 
        }
        else {
            // A user just turned off-line
            auto sender = onlineUsers[fd];
            this->onlineUsers.erase(fd);

            // send the offline message to all
            proto::MessageWrapper offBuf;
            std::strcpy(offBuf.message, this->makeOfflineMsg(sender).c_str());
            offBuf.timestamp = std::time(nullptr);

            // c++17
            for (auto const& [_, receiver] : this->onlineUsers)
                messagePool[receiver].push(offBuf); 
        }
    }

    void SingleThreadServer::writeMessage(int fd) {
        std::string receiver;
        if (
            (receiver = this->onlineUsers[fd]) != "" // not found, (return string's default ctor "")
            && this->messagePool.find(receiver) != this->messagePool.end()
            && !this->messagePool[receiver].empty()
        ) {
            proto::MessageWrapper buf = this->messagePool[receiver].front();
            if (write(fd, reinterpret_cast<char*>(&buf), sizeof(buf)) < 0)
                cnt::errexit("Write message failed: %s\n", strerror(errno));
            this->messagePool[receiver].pop();
        }
    }
}

