#pragma once
#include <WindowConfig.hpp>

namespace win {
    class InputWindow{
    public:
        InputWindow();
        ~InputWindow();
        void show();
        void fill();
        std::string getMessage();
    private:
        WINDOW* win;
        std::string message;
    };
}
