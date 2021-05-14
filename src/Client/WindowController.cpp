#include <WindowController.hpp>

namespace cli {
    WindowController::~WindowController() {
        delwin(this->win);
    }
    
    void WindowController::mainloop() {
        // ######### Initialization #############
        // start ncurses
        // initialize all windows
        this->win = initscr(); // return stdscr
        win::LoginWindow loginWin;


        // initialize network service
        Network network;


        // ######### Print title #############
        // set some option
        noecho();
        curs_set(0); // hide cursor      
        printw("The TCP Chat Room");

        // ######### Login #############
        // retrieve form inputs
        loginWin.show();
        wrefresh(this->win);
        redrawwin(this->win);

        // config connection info
        network.setHost(loginWin.getHost());
        network.setService(loginWin.getService());
        network.setUname(loginWin.getUname());

        network.connect();

        wgetch(this->win);
        endwin();
    }

}

