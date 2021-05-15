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
        win::InputWindow inputWin;
        win::ReceivingWindow recvWin;

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

        // connect to server
        // network.connect();

        // entering chat room
        std::thread inputTh = this->inputController(inputWin);
        recvWin.show();

        wgetch(this->win);
        endwin();
    }
    std::thread WindowController::inputController(win::InputWindow& inputWin) {
        return std::thread( [&, this] { this->ic(inputWin); } );
    }
    void WindowController::ic(win::InputWindow& inputWin) {
        inputWin.show();
    }
}

