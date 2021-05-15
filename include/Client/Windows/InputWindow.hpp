#pragma once
#include <WindowConfig.hpp>

namespace win {
    class InputWindow{
    public:
        InputWindow();
        ~InputWindow();
        void show();
    private:
        WINDOW* win;
    };
}
