/*
 * Author: Ernie Chu
 * Filename: WindowController.hpp
 * Description: A client controller to coordinate the windows and back-end models
 */

#pragma once
#include <Windows.hpp>
#include <Network.hpp>
#include <vector>
#include <future>
#include <chrono>
#include <mutex>
#include <regex>
#include <sstream>
#include <ctime>

namespace cli {
    class WindowController {
    public:
        WindowController();
        ~WindowController();
        void mainloop();
    private:
        std::future<void> inputController();
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

