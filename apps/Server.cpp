#include <SingleThreadServer.hpp>

int main() {
    srv::SingleThreadServer server("1234", 1);
    server.mainloop();
    return 0;
}
