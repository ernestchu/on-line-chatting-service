#include <SingleThreadServer.hpp>

int main() {
    srv::SingleThreadServer test("1234") ;
    test.mainloop(1);
    return 0;
}
