#pragma once

#include <ncurses.h>
#include <string>

namespace win {
    class LoginWindow {
    public:
        LoginWindow();
        ~LoginWindow();
        std::string getHost();
        std::string getService();
        std::string getUname();
        void show();
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
