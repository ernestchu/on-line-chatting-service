#include <ConnectSock.hpp>

namespace cnt {
    int connectSock(
        const char* host,
        const char* service,
        const char* transport,
        char* uinfo
    ) {
        struct hostent      *phe;
        struct servent      *pse;
        struct protoent     *ppe;
        struct sockaddr_in  sin;    /* an Internet endpoint address */
        int s, type;

        memset(&sin, 0, sizeof(sin)); /* clear sin */
        sin.sin_family = AF_INET;

        /* Map service name to port number, allowing for number directly */
        if ((pse = getservbyname(service, transport)))
            sin.sin_port = pse->s_port;
        else if ((sin.sin_port = htons((u_short)atoi(service))) == 0)
            errexit("can't get \"%s\" service entry\n", service);

        /* Map host name to IP address, allowing for dotted decimal */
        if ((phe = gethostbyname(host)))
            memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
        else if ((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE)
            errexit("can't get \"%s\" host entry\n", host);

        /* Map protocol name to protocol number */
        if ((ppe = getprotobyname(transport)) == 0)
            errexit("can't get \"%s\" protocol entry\n", transport);

        /* Use protocol to choose a socket type */
        if (strcmp(transport, "udp") == 0)
            type = SOCK_DGRAM;
        else
            type = SOCK_STREAM;

        /* Allocate a socket */
        s = socket(PF_INET, type, ppe->p_proto);
        if (s < 0)
            errexit("can't create socket: %s\n", strerror(errno));

        /* Connect the socket */
        if (connect(s, (struct sockaddr*)&sin, sizeof(sin)) < 0)
            errexit("can't connect to %s:%s: %s\n", host, service, strerror(errno));
            
        sprintf(uinfo, "%s:%hu", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
        return s;
    }
}

