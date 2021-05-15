#pragma once
#include <WindowConfig.hpp>

namespace win {
    class ReceivingWindow{
    public:
        ReceivingWindow();
        ~ReceivingWindow();
        void show();
    private:
        WINDOW* win;
    };
}
