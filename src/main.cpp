#include <ncurses.h>
#include "gameMenu.h"
#include "mazeGeneration.h"

int main()
{
    initscr();
    noecho();
    curs_set(0);
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    refresh();
    if(gameMenu(yMax, xMax))
    {
        erase();
        generateMaze();
        getch();
    }
    endwin();
    return 0;
}