#include <WindowController.hpp>

namespace cli {
    WindowController::~WindowController() {
        delwin(this->win);
    }
    
    void WindowController::mainloop() {
        // start ncurses
        // initialize all windows
        this->win = initscr(); // return stdscr
        win::LoginWindow loginWin;

        // set some option
        noecho();
        curs_set(0); // hide cursor      

        printw("The TCP Chat Room");

        wgetch(this->win);
        loginWin.show();
        wrefresh(this->win);
        redrawwin(this->win);

        printw("%s", loginWin.getHost().c_str());
        printw("%s", loginWin.getService().c_str());
        printw("%s", loginWin.getUname().c_str());

        wgetch(this->win);
        endwin();
    }

}

