#include <Network.hpp>

namespace cli {
    Network::~Network() { 
        shutdown(this->fd, 2);
        close(fd);
    }
    std::string Network::connect() {
        char uinfo[25];
        this->fd = cnt::connectTCP(
            this->host.c_str(),
            this->service.c_str(),
            uinfo
        );
        proto::MessageWrapper buf;
        std::strcpy(buf.uname, this->uname.c_str());
        if (write(this->fd, reinterpret_cast<char*>(&buf), sizeof(buf)) < 0)
            cnt::errexit("Write message failed: %s\n", strerror(errno));

        // send master public key
        char mpk_cstr[1024];
        int nbytes = read(fd, mpk_cstr, sizeof(mpk_cstr));
        if (nbytes < 0)
            cnt::errexit("Read master public key failed: %s\n", strerror(errno));
        this->mpk = std::string(mpk_cstr);

        // send the private (secret) key based on the user name
        char sk_cstr[1024];
        nbytes = read(fd, sk_cstr, sizeof(sk_cstr));
        if (nbytes < 0)
            cnt::errexit("Read master public key failed: %s\n", strerror(errno));
        this->sk = std::string(sk_cstr);

        return std::string(uinfo);
    }

    void Network::send(
        const std::string& receiver,
        const std::string& message,
        const int noSendBack
    ) {
        proto::MessageWrapper buf;
        std::strcpy(buf.uname, receiver.c_str());
        std::strcpy(buf.message, message.c_str());
        buf.timestamp = std::time(nullptr);
        buf.noSendBack = noSendBack;

        if (write(this->fd, reinterpret_cast<char*>(&buf), sizeof(buf)) < 0)
            cnt::errexit("Write message failed: %s\n", strerror(errno));
    }

    void Network::sendUsage() {
        proto::MessageWrapper buf;
        std::strcpy(buf.uname, "illegal");
        std::strcpy(
            buf.message,
            "\nUsage: <command> [<receiver> ...] [\"<message>\"]"
            "\nExamples:"
            "\n\tsend Ernie \"May I ask for your LINE?\""
            "\n\tlist"
            "\n\tlogout"
        );
        buf.timestamp = std::time(nullptr);

        if (write(this->fd, reinterpret_cast<char*>(&buf), sizeof(buf)) < 0)
            cnt::errexit("Write message failed: %s\n", strerror(errno));
    }
    proto::MessageWrapper Network::receive() {
        proto::MessageWrapper buf;
        int nbytes = read(this->fd, reinterpret_cast<char*>(&buf), sizeof(buf));

        if (nbytes < 0)
            cnt::errexit("Read message failed: %s\n", strerror(errno));

        return buf;
    }

    
    void Network::setHost(std::string host) { this->host = host; }
    void Network::setService(std::string service) { this->service = service; }
    void Network::setUname(std::string uname) { this->uname = uname; }
    std::string Network::getMpk() { return this->mpk; }
    std::string Network::getSk() { return this->sk; }
}
