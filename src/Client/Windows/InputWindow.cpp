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
    void InputWindow::show(int clr) {
        if (clr) {
            werase(this->win);
            box(this->win, 0, 0);
        }
        wrefresh(this->win);
    }
    void InputWindow::fill() {
        // Fill the message
        echo();
        nocbreak();
 
        std::string buf;
        mvwprintw(this->win, 1, 1, "$ ");
        int ch = wgetch(this->win);
        while (ch != '\n') {
            buf.push_back(ch);
            ch = wgetch(this->win);
        }
        this->message = buf;

        cbreak();
        noecho();
    }
    std::string InputWindow::getMessage() { return this->message; }

}
