#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "gameMenu.h"
#include "mazeGeneration.h"
#include "rayCaster.h"

void displayWin()
{
    int maxX = getmaxx(stdscr);
    WINDOW *winWin = newwin(3, maxX/2, getmaxy(stdscr)/4, maxX/4);
    box(winWin, 0, 0);
    mvwprintw(winWin, 1, 1, "You won");
    wgetch(winWin);
    delwin(winWin);
}

void displayExit()
{
    int maxX = getmaxx(stdscr);
    WINDOW *exitWin = newwin(3, maxX/2, getmaxy(stdscr)/4, maxX/4);
    box(exitWin, 0, 0);
    mvwprintw(exitWin, 1, 1, "Thanks for playing");
    wgetch(exitWin);
    delwin(exitWin);
}

void startGame()
{
    //Get maze size
    erase();
    int size;
    echo();
    printw("Enter maze size: ");
    scanw("%d", &size);
    erase();
    noecho();

    //Choose random start and end coordinates
    srand(time(NULL));
    int startY = (rand() % size) * 2 + 1;
    int endY = (rand() % size) * 2 + 1;

    //Create map
    char **map = new char*[size];
    for(int i = 0; i < size; i++)
    {
        map[i] = new char[size];
    }

    //Generate maze
    map = generateMaze(size, startY, endY);
    size = size * 2 + 1;
    
    //Setup functions for ray caster
    cbreak();
    nodelay(stdscr, true);

    //Start ray caster
    rayCaster(map, size, startY, endY);

    //Clean window and display win message
    erase();
    refresh();
    displayWin();   
}

void gameLogic()
{
    //Setup functions for ncurses
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    raw();

    //Get console dimensions
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    refresh();

    //Display menu
    if(gameMenu(yMax, xMax))
    {
        
        startGame();        
    }

    //Display exit on end of game
    displayExit();

    //Exit ncurses mode
    endwin();
}