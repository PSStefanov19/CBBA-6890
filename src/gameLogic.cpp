#include <ncurses.h>
#include "mazeGeneration.h"
#include "rayCaster.h"

void gameLogic()
{
    erase();
    int size;
    echo();
    printw("Enter maze size: ");
	scanw("%d", &size);
	erase();
    noecho();

    char **map = new char*[size];
    for(int i = 0; i < size; i++)
    {
        map[i] = new char[size];
    }

    map = generateMaze(size);
    size = size * 2 + 1;
    
    cbreak();
    nodelay(stdscr, true);
    rayCaster(map, size);
}