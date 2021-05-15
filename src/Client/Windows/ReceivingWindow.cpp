# include <ReceivingWindow.hpp>

namespace win {
    ReceivingWindow::ReceivingWindow() {
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        double padding = (yMax>xMax)? yMax: xMax;
        padding *= WindowConfig::paddingRatio;
        this->win = newwin(
            (int)std::ceil(yMax - padding*2) - WindowConfig::inputLines,
            (int)std::ceil(xMax - padding*2),
            (int)padding,
            (int)padding
        );
        box(this->win, 0, 0);
    }
    ReceivingWindow::~ReceivingWindow() {
        delwin(this->win);
    }
    void ReceivingWindow::show() {
        wrefresh(this->win);
        wgetch(this->win);
    }

}
