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
        this->network.connect();

        // ######### Enter chat room #############

        // ################## Input thread ######################
        std::thread inputTh = this->inputController();

        // ######### Main thread (handle receiving) #############
        // since show() updates stdout, it must be atomic
        while (1) {
            this->mu.lock();
            recvWin.show();
            this->mu.unlock();

            proto::MessageWrapper mw = this->network.receive();
            this->mu.lock();
            this->recvWin.printMessage(mw);
            recvWin.show();
            this->mu.unlock();

            // check if the user has entered `logout`
            if (inputTh.joinable()) {
                inputTh.join();
                break;
            }
        }
        endwin();
    }
    std::thread WindowController::inputController() {
        // spawn a new thread
        return std::thread( [this] { this->ic(); } );
    }
    void WindowController::ic() {
        // real implementation for inputController()
        while (1) {
            this->mu.lock();
            this->inputWin.show(1);
            this->mu.unlock();

            this->inputWin.fill();
            std::string message;
            std::vector<std::string> receivers;
            int cmd = this->parseMessage(
                this->inputWin.getMessage(),
                message,
                receivers
            );
            if (cmd == 0)       // illegal
                this->network.sendUsage();
            else if (cmd == 1)  // logout
                return;
            else if (cmd == 2)  // chat
                for (const auto& receiver : receivers)
                    this->network.send(receiver, message);
        }
    }
    int WindowController::parseMessage(
        const std::string& rawMessage,
        std::string& message,
        std::vector<std::string>& receivers
    ) {
        //test if the message format is legal
        std::regex rule("(chat ([^ ]* )+\\\".+\\\"|logout)");
        if (!std::regex_match(rawMessage, rule))
            return 0;

        // which command
        std::regex cmdRule("(chat|logout)");
        std::smatch match;
        std::regex_search(rawMessage, match, cmdRule);
        std::string cmd = match.str();

        if (cmd == "logout")
            return 1;

        // extract real message;
        std::regex msgRule("\\\".+\\\"");
        std::regex_search(rawMessage, match, msgRule);
        message = match.str();
        // get rid of ""
        message.erase(0, 1);
        message.erase(message.size() - 1);

        // tokenize receivers
        std::string sub = rawMessage.substr(cmd.size()+1);
        sub = sub.substr(0, sub.size()-match.str().size()-1);

        std::istringstream iss(sub, std::istringstream::in);
        std::string receiver;
        while( iss >> receiver )
            receivers.push_back(receiver);

        return 2;
    }
}

