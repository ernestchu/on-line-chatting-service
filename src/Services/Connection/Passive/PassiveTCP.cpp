#include <PassiveTCP.hpp>

namespace cnt {
    int passiveTCP(
        const char* service,
        int qlen,
        int log
    ) {
        return passiveSock(service, "tcp", qlen, log);
    }
}

