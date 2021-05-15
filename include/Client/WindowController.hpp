#pragma once
#include <Windows.hpp>
#include <Network.hpp>
#include <thread>
#include <mutex>

namespace cli {
    class WindowController {
    public:
        WindowController();
        ~WindowController();
        void mainloop();
    private:
        std::thread inputController();
        void ic();

        WINDOW *win;
        std::mutex mu;

        win::LoginWindow loginWin;
        win::InputWindow inputWin;
        win::ReceivingWindow recvWin;
        Network network;
    };
}

