#include <ncurses.h>
#include <string>
#include "gameMenu.h"

/**
 * @brief Initial print of menu
 * @details Prints the four main options and highlights the "> Play" option
 * 
 * @param menu The window to be printed to
 */
void initMenu(WINDOW *menu)
{
	mvwprintw(menu, 0, 1, " Maze Game ");
	wattron(menu, A_REVERSE);
	mvwprintw(menu, 2, 2, "> Play");
	wattroff(menu, A_REVERSE);
	mvwprintw(menu, 4, 2, "> About us");
    mvwprintw(menu, 6, 2, "> Exit");
}

/**
 * @brief A function to print the menu
 * @details Prints the title. Check which the highlighted option is and
 *  highlights it then refreshes the screen
 * 
 * @param menu The window to be printed to
 * @param choice An array of menu options
 * @param highlight Holds the number to the highlighted option
 */
void printMenu(WINDOW *menu ,std::string *choice, int *highlight)
{
	mvwprintw(menu, 0, 1, " Maze Game ");
	for(int i = 0; i < 3; i++)
	{
		if(*highlight == i)
		{
			wattron(menu, A_REVERSE);
				mvwprintw(menu, i*2+2, 2, choice[i].c_str());
			wattroff(menu, A_REVERSE);
		}
		else
		{
			mvwprintw(menu, i*2+2, 2, choice[i].c_str());
		}
	}
	wrefresh(menu);
}


/**
 * @brief Input handler
 * @details Takes input from the keypad and either moves the highlight up, down,
 *  or if keypad enter is pressed switches menu
 * 
 * @param menu The window to be printed to
 * @param highlight Holds the number to the highlighted option
 * @param bSelectedChoice A flag to check if enter has been pressed
 */
void handleInput(WINDOW *menu,int *highlight, bool *bSelectedChoice)
{
	switch(wgetch(menu))
  	{
      	case KEY_UP:
        	*highlight -= 1;
        	if(*highlight == -1)
        	{
            	*highlight = 0;
        	}
        	break;
    
      	case KEY_DOWN:
        	*highlight += 1;
        	if(*highlight == 3)
        	{
            	*highlight = 2;
        	}
        	break;
        case KEY_RIGHT:
        	*bSelectedChoice = true;
        	break;
  	}
}

/**
 * @brief Displays the "About us" option in the menu
 * @details Clears the window. Creates a box and displays information
 * 
 * @param menu The window to be printed to
 */
void displayAboutUs(WINDOW *menu)
{
	werase(menu);
	box(menu, 0, 0);
	mvwprintw(menu, 0, 1, " << About Us ");
	mvwprintw(menu, 2, 2, "A raycasting maze game");
	mvwprintw(menu, 4, 2, "Made by team CBBA-6890");
}

/**
 * @brief Main menu function
 * @details Contains the menu window, menu options and flags.
 *  It initializes the menu and starts the main menu loop. At the end it clears the screen.
 * 
 * @param y Max Y for stdscr
 * @param x Max X for stdscr
 */
bool gameMenu(int y, int x)
{
	WINDOW *menu = newwin(y / 2, x / 2, y / 4, x / 4);
	std::string choices[3] = {"> Play", "> About us", "> Exit"};
    box(menu, 0, 0);
    int highlight = 0;
    bool bSelectedChoice = false;
    keypad(menu, TRUE);
  	initMenu(menu);
  	while(true)
  	{
  		box(menu, 0, 0);
  		printMenu(menu, choices, &highlight);
  		if(!bSelectedChoice)
  		{
  			handleInput(menu, &highlight, &bSelectedChoice);
  		}
  		else
  		{
  			switch(highlight)
			{
				case 0:
					return true;
					break;
				case 1:
					displayAboutUs(menu);
					if(wgetch(menu) == KEY_LEFT)
					{
						bSelectedChoice = false;
					}
					wrefresh(menu);
					break;
				case 2:
					return false;
					break;
			}
  		}
  		werase(menu);
  	}
}