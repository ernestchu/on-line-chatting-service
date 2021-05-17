#include <SingleThreadServer.hpp>
#include <MultiThreadServer.hpp>

void printUsage();

int main(int argc, char *argv[]) {
    int type = 0;
    std::string service;
    switch (argc) {
        case 2:
            service = argv[1];
            break;
        case 3:
            if (argv[1][0] == '-') {
                if (argv[1][1] == 'p')
                    type = 1;
                else if (argv[1][1] == 't')
                    type = 2;
                else
                    printUsage();
            service = argv[2];
            break;
            } else
                printUsage();
            break;
        default:
            printUsage();
    }
    std::unique_ptr<srv::AbstractServer> server;
    if (type == 0)
        server = std::unique_ptr<srv::AbstractServer>(
            new srv::SingleThreadServer(service.c_str(), 1)
        );
    // else if (type == 1)
    //     server = std::unique_ptr<srv::AbstractServer>(
    //         new srv::MultiProcessServer(service.c_str(), 1)
    //     );
    else if (type == 2)
        server = std::unique_ptr<srv::AbstractServer>(
            new srv::MultiThreadServer(service.c_str(), 1)
        );

    server->mainloop();
    return 0;
}

void printUsage() {
    std::cout << 
        "\nNAME:\n"
        "\tServer -- start a on-line chatting server\n"
        "SYNOPSIS\n"
        "\t./Server [-pt] <port number | service name>\n"
        "DESCRIPTION\n"
        "\tThe Server program use select() to run a concurrent program in defualt.\n"
        "\tOne can specify -p to run the server in multiple processes.\n"
        "\tOr specify -t to run the server in multiple threads\n\n";
    exit(1);
}
