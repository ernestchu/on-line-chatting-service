# include <InputWindow.hpp>

namespace win {
    InputWindow::InputWindow() {
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        double padding = (yMax>xMax)? yMax: xMax;
        padding *= WindowConfig::paddingRatio;
        this->win = newwin(
            WindowConfig::inputLines,
            (int)std::ceil(xMax - padding*2),
            (int)std::ceil(yMax - padding) - (WindowConfig::inputLines+1),
            (int)padding
        );
        box(this->win, 0, 0);
    }
    InputWindow::~InputWindow() {
        delwin(this->win);
    }
    void InputWindow::show() {
        wrefresh(this->win);
    }
    void InputWindow::fill() {
        // Fill the message
        echo();
        nocbreak();
        curs_set(1); // show cursor      
 
        std::string buf;
        int ch = mvwgetch(this->win, 1, 1);
        while (ch != '\n') {
            buf.push_back(ch);
            ch = wgetch(this->win);
        }
        this->message = buf;
    }
    std::string InputWindow::getMessage() { return this->message; }

}
