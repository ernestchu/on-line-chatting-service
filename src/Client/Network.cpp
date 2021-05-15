#include <Network.hpp>

namespace cli {
    void Network::connect() {
        this->fd = cnt::connectTCP(
            this->host.c_str(),
            this->service.c_str()
        );
        proto::MessageWrapper buf;
        std::strcpy(buf.uname, this->uname.c_str());
        if (write(this->fd, reinterpret_cast<char*>(&buf), sizeof(buf)) < 0)
            cnt::errexit("Write message failed: %s\n", strerror(errno));

    }

    void Network::send(
        const std::string& receiver,
        const std::string& message
    ) {
        proto::MessageWrapper buf;
        std::strcpy(buf.uname, receiver.c_str());
        std::strcpy(buf.message, message.c_str());
        buf.timestamp = std::time(nullptr);

        if (write(this->fd, reinterpret_cast<char*>(&buf), sizeof(buf)) < 0)
            cnt::errexit("Write message failed: %s\n", strerror(errno));
    }

    void Network::sendUsage() {
        proto::MessageWrapper buf;
        std::strcpy(buf.uname, this->uname.c_str());
        std::strcpy(
            buf.message,
            "Usage: <command> [<receiver> ...] [\"<message>\"]\n"
            "Command: chat, logout\n"
            "\tE.g. chat Ernie \"Good Program!\""
        );
        buf.timestamp = std::time(nullptr);

        if (write(this->fd, reinterpret_cast<char*>(&buf), sizeof(buf)) < 0)
            cnt::errexit("Write message failed: %s\n", strerror(errno));
    }

    
    void Network::setHost(std::string host) { this->host = host; }
    void Network::setService(std::string service) { this->service = service; }
    void Network::setUname(std::string uname) { this->uname = uname; }
}
