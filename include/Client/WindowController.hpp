#pragma once
#include <Windows.hpp>

namespace cli {
    class WindowController {
    public:
        ~WindowController();
        void mainloop();
    private:
        WINDOW *win;
    };
}

