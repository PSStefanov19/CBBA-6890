#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "gameMenu.h"
#include "mazeGeneration.h"
#include "rayCaster.h"

/**
 * @brief Function that displays the win message
 * 
 */
void displayWin()
{
    //Get max console height
    int maxX = getmaxx(stdscr);

    //Create a window that will contain the win message
    WINDOW *winWin = newwin(3, maxX/2, getmaxy(stdscr)/4, maxX/4);

    //Create a box around the win window
    box(winWin, 0, 0);

    //Display the win message
    mvwprintw(winWin, 1, 1, "You won");

    //Wait for input
    wgetch(winWin);

    //Delete the window
    delwin(winWin);
}

/**
 * @brief Function that displays the exit message
 * 
 */
void displayExit()
{
    //Get max console height
    int maxX = getmaxx(stdscr);

    //Create a window that will contain the exit message
    WINDOW *exitWin = newwin(3, maxX/2, getmaxy(stdscr)/4, maxX/4);

    //Create a box around the exit wondow
    box(exitWin, 0, 0);

    //Print the exit message
    mvwprintw(exitWin, 1, 1, "Thanks for playing");

    //Wait for input
    wgetch(exitWin);

    //Delete the window
    delwin(exitWin);
}

/**
 * @brief Function that initializes the game
 * 
 */
void startGame()
{
    //Clear the screen
    erase();

    //Turn on printing written characters to screen
    echo();

    //Get maze size
    int size;
    printw("Enter maze size: ");
    scanw("%d", &size);

    //Turn off printing written characters to screen
    noecho();

    //Clear the screen
    erase();

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

/**
 * @brief A function that holds all the logic to the game
 * 
 */
void gameLogic()
{
    //Setup functions for ncurses
    initscr();

    //Stop character from displaying on the screen
    noecho();

    //Make cursor invisible
    curs_set(0);

    //Enable keypad input
    keypad(stdscr, true);

    //Give the raw input without the computer reworking it
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