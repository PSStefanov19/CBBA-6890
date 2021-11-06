#include <ncurses.h>
#include "gameMenu.h"
#include "gameLogic.h"

int main()
{
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    raw();
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    refresh();
    if(gameMenu(yMax, xMax))
    {
        gameLogic();
    }
    endwin();
    return 0;
}