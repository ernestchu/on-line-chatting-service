#include <WindowController.hpp>

namespace cli {
    // start ncurses and initialize the members with default ctor
    WindowController::WindowController() : win(initscr()) {}
    WindowController::~WindowController() {
        delwin(this->win);
    }
    
    void WindowController::mainloop() {
        // set some option
        noecho();
        curs_set(0); // hide cursor      
        // Print title
        printw("The TCP Chat Room");
        wrefresh(this->win); // update stdout

        // ######### Login #############
        // retrieve form inputs
        this->loginWin.show();
        this->loginWin.fill();
        // done
        // hide (overridden by stdscr) login window
        redrawwin(this->win); 

        // ######### Connect #############
        // config connection info
        this->network.setHost(loginWin.getHost());
        this->network.setService(loginWin.getService());
        this->network.setUname(loginWin.getUname());

        // connect to server
        // this->network.connect();

        // ######### Enter chat room #############

        // ################## Input thread ######################
        std::thread inputTh = this->inputController();

        // ######### Main thread (handle receiving) #############
        // since show() updates stdout, it must be atomic
        this->mu.lock();
        recvWin.show();
        this->mu.unlock();

        inputTh.join();

        endwin();
    }
    std::thread WindowController::inputController() {
        // spawn a new thread
        return std::thread( [this] { this->ic(); } );
    }
    void WindowController::ic() {
        // real implementation for inputController()
        this->mu.lock();
        this->inputWin.show();
        this->mu.unlock();

        this->inputWin.fill();
    }
}

