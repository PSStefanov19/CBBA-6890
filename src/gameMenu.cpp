#include <ncurses.h>
#include <string>
#include "gameMenu.h"

void initMenu(WINDOW *menu)
{
	mvwprintw(menu, 0, 1, " Maze Game ");
	wattron(menu, A_REVERSE);
	mvwprintw(menu, 2, 2, "> Play");
	wattroff(menu, A_REVERSE);
	mvwprintw(menu, 4, 2, "> About us");
    mvwprintw(menu, 6, 2, "> Exit");
}

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
        case KEY_ENTER:
        	*bSelectedChoice = true;
        	break;
  	}
}


void diplayAboutUs(WINDOW *menu)
{
	werase(menu);
	box(menu, 0, 0);
	mvwprintw(menu, 0, 1, " About Us ");
	mvwprintw(menu, 2, 2, "A raycasting maze game");
	mvwprintw(menu, 4, 2, "Made by team CBBA-6890");
}

void gameMenu(int y, int x)
{
	WINDOW *menu = newwin(y / 2, x / 2, y / 4, x / 4);
	std::string choices[3] = {"> Play", "> About us", "> Exit"};
    box(menu, 0, 0);
    int highlight = 0;
    bool bSelectedChoice = false;
    bool bRunning = true;
    keypad(menu, TRUE);
  	initMenu(menu);
  	while(bRunning)
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
					bRunning = false;
					break;
				case 1:
					diplayAboutUs(menu);
					if(wgetch(menu) == KEY_ENTER)
					{
						bSelectedChoice = false;
					}
					wrefresh(menu);
					break;
				case 2:
					bRunning = false;
					endwin();
					break;
			}
  		}
  		werase(menu);
  	}
	delwin(menu);
}