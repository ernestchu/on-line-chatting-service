#include <SingleThreadServer.hpp>
#include <MultiThreadServer.hpp>

void printUsage();

int main(int argc, char **argv) {
    int tFlag = 0;
    int vFlag = 0;
    int error = 0;
    char* service;
    int c;

    while ((c = getopt(argc, argv, "htv")) != -1)
        switch (c) {
            case 'h':
                printUsage();
                break;
            case 't':
                tFlag = 1;
                break;
            case 'v':
                vFlag = 1;
                break;
            case '?':
                error = 1;
        }

    if ((argc-optind) == 1)
        service = argv[optind];
    else if ((argc-optind) == 0) {
        std::cerr << "You must specify the port number or service name.\n";
        error = 1;
    } else {
        std::cerr << "Too many arguments.\n";
        error = 1;
    }

    if (error)
        printUsage();


    std::unique_ptr<srv::AbstractServer> server;
    if (tFlag)
        server = std::unique_ptr<srv::AbstractServer>(
            new srv::MultiThreadServer(service, vFlag)
        );
    else
        server = std::unique_ptr<srv::AbstractServer>(
            new srv::SingleThreadServer(service, vFlag)
        );

    server->mainloop();
    return 0;
}

void printUsage() {
    std::cout << 
        "\nSYNOPSIS\n"
        "\t./Server [-htv] <port number | service name>\n"
        "DESCRIPTION\n"
        "\tThe Server program use select() to run a concurrent program in defualt.\n"
        "\tOne can specify -t to run the server in multiple threads.\n"
        "OPTIONS\n"
        "\t-h\tShow help\n"
        "\t-t\tRun the server in multi-threaded mode.\n"
        "\t-v\tProduce verbose output.\n\n";
    exit(1);
}
