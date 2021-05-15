#pragma once
#include <WindowConfig.hpp>

namespace win {
    class ReceivingWindow{
    public:
        ReceivingWindow();
        ~ReceivingWindow();
        void show();
        void printMessage(proto::MessageWrapper mw);
    private:
        WINDOW* win;
    };
}
