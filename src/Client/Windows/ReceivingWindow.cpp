# include <ReceivingWindow.hpp>

namespace win {
    ReceivingWindow::ReceivingWindow() {
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        double padding = (yMax>xMax)? yMax: xMax;
        padding *= WindowConfig::paddingRatio;
        this->win = newwin(
            (int)std::ceil(yMax - padding*2) - WindowConfig::inputLines - 1,
            (int)std::ceil(xMax - padding*2),
            (int)padding,
            (int)padding
        );
        box(this->win, 0, 0);
        scrollok(this->win, TRUE); // for scrolling
    }
    ReceivingWindow::~ReceivingWindow() {
        delwin(this->win);
    }
    void ReceivingWindow::show() {
        wrefresh(this->win);
    }
    void ReceivingWindow::printMessage(proto::MessageWrapper mw) {
        int yBeg, xBeg, yMax, xMax;
        getbegyx(this->win, yBeg, xBeg);
        getmaxyx(this->win, yMax, xMax);
        int nlines = yMax - yBeg;
        int ncols = xMax - xBeg;
        auto printLine = [&] (std::string msg, bool alignL=false) {
            scroll(this->win);
            wmove(this->win, nlines, 0);
            wclrtoeol(this->win);
            box(this->win, 0, 0);
            if (alignL)
                mvwprintw(this->win, nlines-1, ncols-msg.size(), "%s", msg.c_str());
            else
                mvwprintw(this->win, nlines-1, 2, "%s", msg.c_str());
        };
        std::string sender = mw.uname;
        std::string message = mw.message;
        message = sender + ": " + message;
        std::string datetime = std::ctime(&mw.timestamp);
        // get rid of new line
        datetime = datetime.substr(0, datetime.size()-1);

        // print the message line by line
        std::size_t LF;
        while (
            (LF=message.find("\n")) != std::string::npos 
            && LF != message.size()-1
        ) {
            std::string line = message.substr(0, LF);
            message = message.substr(LF+1);
            printLine(line);
        }
        // check if there's trailing string
        if (
            message.find("\n") == std::string::npos
            && message.size()
        )
            printLine(message);

        printLine(datetime, true);

    }

}
