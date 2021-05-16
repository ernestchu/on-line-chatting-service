#include <PassiveSock.hpp>

extern int errno;

namespace cnt {
    /* port base, for non-root servers */
    unsigned short portbase = 0;

    int passiveSock(
        const char* service,
        const char* transport,
        int qlen,
        int log
    ) {
        struct servent      *pse;
        struct protoent     *ppe;
        struct sockaddr_in  sin;
        int s, type;

        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
        
        /* Map service name to a port number */
        if ((pse = getservbyname(service, transport)))
            sin.sin_port = htons(ntohs((unsigned short)pse->s_port) + portbase);
        else if ((sin.sin_port = htons((unsigned short)atoi(service))) == 0)
            errexit("can't get \"%s\" service entry\n", service);

        /* Map protocol name to protocol number */
        if ((ppe = getprotobyname(transport)) == 0)
            errexit("can't get \"%s\" protocol entry\n", transport);

        /* Use protocol to choose a socket type */
        if (strcmp(transport, "udp") == 0) type = SOCK_DGRAM;
        else type = SOCK_STREAM;

        /* Allocate a socket */
        if ((s = socket(PF_INET, type, ppe->p_proto)) < 0)
            errexit("can't create socket: %s\n", strerror(errno));

        /* Bind the socket */
        if (bind(s, (struct sockaddr*)&sin, sizeof(sin)) < 0)
            errexit("can't bind to %s port: %d\n", service, strerror(errno));

        if (type == SOCK_STREAM && listen(s, qlen) < 0)
            errexit("can't listen on %s port : %s\n", service, strerror(errno));

        if (log)
            printHostAddrs(ntohs(sin.sin_port));

        return s;
    }

    void printHostAddrs(unsigned short port) {
        struct ifaddrs *addresses;
        if (getifaddrs(&addresses) == -1)
            errexit("getifaddrs failed: %s\n", strerror(errno));

        std::cout << "Starting server.\nListen on:\n";
        struct ifaddrs *address = addresses;
        while(address) { /* all network interface */
            int family = address->ifa_addr->sa_family;
            if (family == AF_INET) {
                char ap[100];
                const int family_size = sizeof(sockaddr_in);
                getnameinfo(address->ifa_addr,family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
                std::cout << "\t" << ap << ":" << port << std::endl;
            }
            address = address->ifa_next;
        }
        freeifaddrs(addresses);
    }
}

