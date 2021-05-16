#include <ConnectUDP.hpp>

namespace cnt {
    int connectUDP(const char* host, const char* service, char* uinfo) {
        return connectSock(host, service, "udp", uinfo);
    }
}

