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

}
