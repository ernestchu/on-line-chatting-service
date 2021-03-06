/*
 * Author: Ernie Chu
 * Filename: ReceivingWindow.hpp
 * Description: A window for displaying the received messages in the chat room
 */

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
