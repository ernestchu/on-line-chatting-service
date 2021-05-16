#include <Network.hpp>

namespace cli {
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

        return std::string(uinfo);
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
            "\nUsage: <command> [<receiver> ...] [\"<message>\"]"
            "\nExamples:"
            "\n\tchat Ernie \"May I ask for your LINE?\""
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
}
