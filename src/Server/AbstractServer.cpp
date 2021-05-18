#include <AbstractServer.hpp>

namespace srv {
    AbstractServer::AbstractServer(const char* service) :
        service(service),
        log(0) {}
    AbstractServer::AbstractServer(const char* service, const int log) :
        service(service),
        log(log) {}

    void AbstractServer::addNewClient(const struct sockaddr_in& sin, const int& fd) {
        // first time information acquisition
        proto::MessageWrapper buf; // buf.uname is the new client's name
        int nbytes = read(fd, reinterpret_cast<char*>(&buf), sizeof(buf));
        if (nbytes < 0)
            cnt::errexit("Read message failed: %s\n", strerror(errno));
        if (this->log)
            this->messageLog(buf);

        proto::MessageWrapper onBuf;
        std::strcpy(onBuf.uname, "System:");
        std::strcpy(onBuf.message, this->makeOnlineMsg(sin, buf.uname).c_str());
        onBuf.timestamp = std::time(nullptr);
        this->broadcast(onBuf);
 
        this->mu.lock();
        this->registeredUsers.insert(buf.uname);
        this->onlineUsers[fd] = buf.uname;
        this->mu.unlock();

        this->mu.lock();
        if (this->log)
            std::cout << onBuf.message << std::endl;
        this->mu.unlock();
    }

    int AbstractServer::readMessage(const int& fd) {
        proto::MessageWrapper buf; // buf.uname is whom fd wants to send to

        int nbytes = read(fd, reinterpret_cast<char*>(&buf), sizeof(buf));
        if (this->log)
            this->messageLog(buf);

        if (nbytes < 0)
            cnt::errexit("Read message failed: %s\n", strerror(errno));
        else if (nbytes > 0) {
            // Normal message transmission
            this->mu.lock();
            const std::string sender   = this->onlineUsers[fd];
            const std::string receiver = buf.uname;
            std::strcpy(buf.uname, sender.c_str());

            if (receiver == "illegal") {
                // send the usage, which is generated by the front end 
                std::strcpy(buf.uname, "System");
                this->messagePool[sender].push(buf); 
            }
            else {
                // send back the orginal message to the sender
                this->messagePool[sender].push(buf);

                // make a set of online users
                std::unordered_set<std::string> online;
                // c++17
                for (auto const& [_, v] : this->onlineUsers)
                    online.insert(v);

                if (receiver == "System") { // System message, list, logout, etc.
                    std::strcpy(
                        buf.message, 
                        this->systemResponse(std::string(buf.message)).c_str()
                    );
                    std::strcpy(buf.uname, "System");
                    this->messagePool[sender].push(buf); // send back to the sender 
                } else {
                    if (this->registeredUsers.find(receiver) == this->registeredUsers.end()) {
                        // receiver not found (not registered)
                        std::strcpy(buf.uname, "System");
                        std::sprintf(buf.message, "%s: user not found", receiver.c_str());
                        this->messagePool[sender].push(buf); 
                    } else if (online.find(receiver) == online.end()) {
                        // receiver exists but is offline
                        this->messagePool[receiver].push(buf);
                        // a note for off-line made by the system
                        std::strcpy(buf.uname, "System");
                        std::sprintf(buf.message, "%s is currently off-line. The message is cached in the server", receiver.c_str());
                        this->messagePool[sender].push(buf); 
                    }
                    else
                        // normal: send to another user
                        this->messagePool[receiver].push(buf);
                }
            }

            this->mu.unlock();
        }
        else { // nbytes == 0
            this->removeClient(fd);
            close(fd);

            // ############### the map is not clean after erase! ##########
            this->mu.lock();
            for (auto const& [k, v] : this->onlineUsers)
                std::cout << "what inside the onlineUsers: " << k << ":" << v << "EOL" << std::endl;
            this->mu.unlock();
        

            return 0; // indicated that the client has leaved
        }
        return 1;
    }

    void AbstractServer::writeMessage(const int& fd) {
        std::string receiver;
        this->mu.lock();
        if (
            (receiver = this->onlineUsers[fd]) != "" // not found, (return string's default ctor "")
            && this->messagePool.find(receiver) != this->messagePool.end()
            && !this->messagePool[receiver].empty()
        ) {
            proto::MessageWrapper buf = this->messagePool[receiver].front();
            this->messagePool[receiver].pop();
            this->mu.unlock();

            if (write(fd, reinterpret_cast<char*>(&buf), sizeof(buf)) < 0)
                cnt::errexit("Write message failed: %s\n", strerror(errno));
            if (this->log)
                this->messageLog(buf, 0);
        } else
            this->mu.unlock();
    }


    std::string AbstractServer::makeOnlineMsg(
        const struct sockaddr_in& sin,
        const std::string& uname
    ) {
        return "User " + uname + " is on-line @" + std::string(inet_ntoa(sin.sin_addr)) + ".";
    }

    std::string AbstractServer::makeOfflineMsg(const std::string& uname) {
        return "User " + uname + " is off-line.";
    }

    void AbstractServer::messageLog(const proto::MessageWrapper& msg, const int recv) {
        this->mu.lock();
        std::cout << ((recv)? "Received message:\n": "Sent message:\n")
                  << "\tUser name: " << msg.uname << std::endl
                  << "\tMessage: " << msg.message << std::endl
                  << "\tTime: " << msg.timestamp << std::endl;
        this->mu.unlock();
    }

    void AbstractServer::broadcast(const proto::MessageWrapper& msg) {
        this->mu.lock();
        // c++17
        for (auto const& [_, receiver] : this->onlineUsers)
            this->messagePool[receiver].push(msg); 
        this->mu.unlock();
    }
    std::string AbstractServer::systemResponse(const std::string& cmd) {
        if (cmd == "list") {
            return this->dumpUsers();
        }
        else if (cmd == "logout")
            return "Bye!";
        else
            return cmd + ": command not found";
    }
    std::string AbstractServer::dumpUsers() {
        std::string dump = "\nOff-line users:";
        std::set<std::string> online, offline, registered;

        // needed, because set_difference doesn't support unordered_set
        for (auto const& [_, v] : this->onlineUsers)
            online.insert(v);
        for (auto const& v : this->registeredUsers)
            registered.insert(v);

        std::set_difference(
            registered.begin(), registered.end(),
            online.begin(), online.end(),
            std::inserter(offline, offline.begin())
        );

        for (auto const& user : offline)
            dump = dump + "\n\t" + user;

        dump += "\nOn-line users:";
        for (auto const& user : online)
            dump = dump + "\n\t" + user;

        return dump;
    }
}

