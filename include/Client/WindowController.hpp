#pragma once
#include <Windows.hpp>
#include <Network.hpp>
#include <thread>

namespace cli {
    class WindowController {
    public:
        ~WindowController();
        void mainloop();
    private:
        WINDOW *win;
        std::thread inputController(win::InputWindow& inputWin);
        void ic(win::InputWindow& inputWin);
    };
}

