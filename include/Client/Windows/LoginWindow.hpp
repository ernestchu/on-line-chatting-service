#pragma once
#include <WindowConfig.hpp>

namespace win {
    class LoginWindow {
    public:
        LoginWindow();
        ~LoginWindow();
        std::string getHost();
        std::string getService();
        std::string getUname();
        void show();
        void fill();
    private:
        WINDOW* win;
        WINDOW* hostInputWin;
        WINDOW* serviceInputWin;
        WINDOW* unameInputWin;
        std::string host;
        std::string service;
        std::string uname;
    };
}
