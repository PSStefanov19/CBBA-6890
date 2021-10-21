#include <ncurses.h>
#include <string>
#include "gameMenu.h"

int main()
{
    initscr();
        noecho();
        curs_set(0);
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        refresh();
        gameMenu(yMax, xMax);
        erase();
        printw("Playing");
        getch();
    endwin();
    return 0;
}