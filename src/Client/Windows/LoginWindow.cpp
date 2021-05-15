# include <LoginWindow.hpp>

namespace win {
    LoginWindow::LoginWindow() {
        // main window
        int yBeg, xBeg, yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        double padding = (yMax>xMax)? yMax: xMax;
        padding *= WindowConfig::paddingRatio;
        this->win = newwin(
            (int)std::ceil(yMax - padding*2),
            (int)std::ceil(xMax - padding*2),
            (int)padding,
            (int)padding
        );
        box(this->win, 0, 0);
        getbegyx(this->win, yBeg, xBeg);
        getmaxyx(this->win, yMax, xMax);
        wattron(this->win, A_REVERSE);
        mvwprintw(
            this->win,
            (int)(yMax/2-7),
            (int)(xMax/2-4),
            "  Login  "
        );
        wattroff(this->win, A_REVERSE);
        int i = 1;
        for (const auto& label : {
            "Host",
            "Service",
            "Username",
        }) {
            mvwprintw(
                this->win,
                (int)(yMax/2-5+(i++)),
                (int)(xMax/2-12-std::strlen(label)),
                label
            );
            i+=2;
        }

        i = 0;
        for (const auto& w : {
            &this->hostInputWin,
            &this->serviceInputWin,
            &this->unameInputWin
        }) {
            *w = subwin(
                this->win,
                3,
                22,
                (int)(yMax/2-5+(i++)+yBeg),
                (int)(xMax/2-10+xBeg)
            );
            box(*w, 0, 0);
            i+=2;
        }
        
    }
    LoginWindow::~LoginWindow() {
        delwin(this->win);
        delwin(this->hostInputWin);
        delwin(this->serviceInputWin);
        delwin(this->unameInputWin);
    }

    std::string LoginWindow::getHost() { return this->host; }
    std::string LoginWindow::getService() { return this->service; }
    std::string LoginWindow::getUname() { return this->uname; }

    void LoginWindow::show() {   
        wrefresh(this->win);
    }
    void LoginWindow::fill() {   
        // Fill the form
        echo();
        nocbreak();
        curs_set(1); // show cursor      
        
        int i = 0;
        for (const auto& w : {
            &this->hostInputWin,
            &this->serviceInputWin,
            &this->unameInputWin
        }) {
            std::string buf;
            int ch = mvwgetch(*w, 1, 1);
            while (ch != '\n') {
                buf.push_back(ch);
                ch = wgetch(*w);
            }
            switch (i++) {
                case 0:
                    this->host = buf;
                    break;
                case 1:
                    this->service = buf;
                    break;
                case 2:
                    this->uname = buf;
                    break;
            }
        }
        // restore options
        noecho();
        cbreak();
        curs_set(0);      
    }
  
}
