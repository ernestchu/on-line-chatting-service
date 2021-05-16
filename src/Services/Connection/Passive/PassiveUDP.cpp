#include <PassiveUDP.hpp>

namespace cnt {
    int passiveUDP(
        const char* service,
        int log
    ) {
        return passiveSock(service, "udp", 0, log);
    }
}

