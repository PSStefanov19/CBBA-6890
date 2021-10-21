#include <ncurses.h>
#include <gameMenu.h>
using namespace std;

int main()
{
    initscr();
    refresh();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_MAGENTA); 
    attron(COLOR_PAIR(1));
    printHello();
    attroff(COLOR_PAIR(1));
    refresh();
    getch();
    endwin();
}