#pragma once
#include <Windows.hpp>
#include <Network.hpp>
#include <vector>
#include <thread>
#include <mutex>
#include <regex>
#include <sstream>

namespace cli {
    class WindowController {
    public:
        WindowController();
        ~WindowController();
        void mainloop();
    private:
        std::thread inputController();
        void ic();
        int parseMessage(
            const std::string& rawMessage,
            std::string& message,
            std::vector<std::string>& receivers
        );

        WINDOW *win;
        std::mutex mu;

        win::LoginWindow loginWin;
        win::InputWindow inputWin;
        win::ReceivingWindow recvWin;
        Network network;
    };
}

