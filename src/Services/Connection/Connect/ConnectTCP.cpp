#include <ConnectTCP.hpp>

namespace cnt {
    int connectTCP(const char* host, const char* service, char* uinfo) {
        return connectSock(host, service, "tcp", uinfo);
    }
}

