#include <ncurses.h>
using namespace std;

int main()
{
    initscr();
    refresh();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_MAGENTA); 
    attron(COLOR_PAIR(1));
    mvprintw(3, 2, "Hello world");
    attroff(COLOR_PAIR(1));
    refresh();
    getch();
    endwin();
}