#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "mazeGeneration.h"
#include "rayCaster.h"

void displayWin()
{
    int maxX = getmaxx(stdscr);;
    WINDOW *winbox = newwin(3, maxX/2, getmaxy(stdscr)/4, maxX/4);
    box(winbox, 0, 0);
    mvwprintw(winbox, 1, 1, "You won");
    wgetch(winbox);
}

void gameLogic()
{
    srand(time(NULL));
    erase();
    int size;
    echo();
    printw("Enter maze size: ");
	scanw("%d", &size);
	erase();
    noecho();
    int startY = (rand() % size) * 2 + 1;
	int endY = (rand() % size) * 2 + 1;


    char **map = new char*[size];
    for(int i = 0; i < size; i++)
    {
        map[i] = new char[size];
    }

    map = generateMaze(size, startY, endY);
    size = size * 2 + 1;
    
    cbreak();
    nodelay(stdscr, true);
    rayCaster(map, size, startY, endY);
    erase();
    refresh();
    displayWin();
}