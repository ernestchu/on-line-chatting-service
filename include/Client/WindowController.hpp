#pragma once
#include <Windows.hpp>
#include <Network.hpp>

namespace cli {
    class WindowController {
    public:
        ~WindowController();
        void mainloop();
    private:
        WINDOW *win;
    };
}

