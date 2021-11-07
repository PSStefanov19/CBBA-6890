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
 * @param highlight Index of highlighted option
 */
void printMenu(WINDOW *menu ,std::string *choice, int *highlight)
{
	//Print the title
	mvwprintw(menu, 0, 1, " Maze Game ");

	//Print all options of menu
	for(int i = 0; i < 3; i++)
	{
		//If i is equal to highlight then highlight it
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
	//Switch that checks the input of the user
	switch(wgetch(menu))
  	{
		//If user presses w, W or KEY_UP move the selection up
		case KEY_UP:
		case 'W':
      	case 'w':
        	*highlight -= 1;

			//If selection is less than 0 return it to 0
        	if(*highlight == -1)
        	{
            	*highlight = 0;
        	}
        	break;
    
		//If user presses s, S or KEY_DOWN move the selection down
		case KEY_DOWN:
		case 'S':
      	case 's':
        	*highlight += 1;

			//If selelction is greater than max options return it to the last option
        	if(*highlight == 3)
        	{
            	*highlight = 2;
        	}
        	break;
		
		//If user pressed d, D or KEY_RIGHT select the choice
		case KEY_RIGHT:
		case 'D':
        case 'd':
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
	//Init a new window
	WINDOW *menu = newwin(y / 2, x / 2, y / 4, x / 4);

	//Options of the menu
	std::string choices[3] = {
		"> Play",
		"> About us", 
		"> Exit"
	};

	//Create a box around the menu
    box(menu, 0, 0);

	//Index of highlighted option
    int highlight = 0;

	//Flag to check if option has been selected
    bool bSelectedChoice = false;

	//Enable keypad usage
    keypad(menu, TRUE);

	//Initialize the menu
  	initMenu(menu);
	
	//Menu loop
  	while(true)
  	{
		//Print the menu
  		box(menu, 0, 0);
  		printMenu(menu, choices, &highlight);
		
		//If nothing has been selected wait for input
  		if(!bSelectedChoice)
  		{
  			handleInput(menu, &highlight, &bSelectedChoice);
  		}
  		else
  		{
			//Switch of chosen options
  			switch(highlight)
			{
				//If option 0 is selecten then start game
				case 0:
					werase(menu);
					return true;
					break;

				//If option 1 is selceted then display about us
				case 1:
					displayAboutUs(menu);
					if(wgetch(menu) == 'a' or wgetch(menu) == 'A' or wgetch(menu) == KEY_LEFT)
					{
						bSelectedChoice = false;
					}
					wrefresh(menu);
					break;

				//If option 2 is selected then exit the game
				case 2:
					delwin(menu);
					return false;
					break;
			}
  		}
		//Erase the menu
  		werase(menu);
  	}
}