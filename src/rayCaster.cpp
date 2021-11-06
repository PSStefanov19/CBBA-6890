#include <ncurses.h>
#include <math.h>
#include "rayCaster.h"

bool checkIfWon(int currectPosX, int currentPosY, int exitX, int exitY)
{
    if(currectPosX == exitX and currentPosY == exitY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void rayCaster(char **maze,int size, int startY, int endY)
{
    int consoleHeight = getmaxy(stdscr);
    int consoleWidth = getmaxx(stdscr);
    int mapWidth = size;
    int mapHeight = size;

    float playerX = 1.0f;
    float playerY = startY;
    float playerAngle = 0.0f;
    float FOV = 3.14159 / 5.5;
    float maxRenderDistance = 10.0f;
    float speed = 1.0f;

    start_color();
    init_pair(SKY, COLOR_BLUE, COLOR_BLUE);
    init_pair(WALLS, COLOR_BLACK, COLOR_WHITE);
    init_pair(FLOOR, COLOR_YELLOW, COLOR_YELLOW);

    while (!(checkIfWon(int(playerX), int(playerY), size-1, endY)))
    {
        switch(getch())
        {
            case KEY_UP:
            case 'W':
            case 'w':
                playerX += sinf(playerAngle) * speed * 0.075;
                playerY += cosf(playerAngle) * speed * 0.075;
                if(maze[(int)playerY][(int)playerX] == '#')
                {
                    playerX -= sinf(playerAngle) * speed * 0.075;
                    playerY -= cosf(playerAngle) * speed * 0.075;
                }
            break;
            case KEY_DOWN:
            case 'S':
            case 's':
                playerX -= sinf(playerAngle) * speed * 0.075;
                playerY -= cosf(playerAngle) * speed * 0.075;
                if(maze[(int)playerY][(int)playerX] == '#')
                {
                    playerX += sinf(playerAngle) * speed * 0.075;
                    playerY += cosf(playerAngle) * speed * 0.075;
                }
            break;
            case KEY_LEFT:
            case 'A':
            case 'a':
                playerAngle -= speed * 0.01;
            break;
            case KEY_RIGHT:
            case 'D':
            case 'd':
                playerAngle += speed * 0.01;
            break;
        }
        for(int x = 0; x < consoleWidth; x++)
        {
            float rayAngle = (playerAngle - FOV/2.0) + ((float)x / (float)consoleWidth) * FOV;

            float stepSize = 0.075f;
            float distanceToWall = 0.0f;

            bool hitWall = false;

            float eyeX = sinf(rayAngle);
            float eyeY = cosf(rayAngle);

            while(!hitWall && distanceToWall < maxRenderDistance)
            {
                distanceToWall += stepSize;

                int testX = (int)(playerX + eyeX * distanceToWall);
                int testY = (int)(playerY + eyeY * distanceToWall);

                if(testX < 0 || testX > mapWidth || testY < 0 || testY > mapHeight)
                {
                    hitWall = true;
                    distanceToWall = maxRenderDistance;
                }
                else 
                {
                    if(maze[testY][testX] == '#')
                    {
                        hitWall = true;
                    }
                }
            }

            int startCeiling = (float)(consoleHeight/2.0) - consoleHeight / ((float)distanceToWall);
            int startFloor = consoleHeight - startCeiling;

            char shade = ' ';

            // .:-=+*#%@
            
            switch((int)distanceToWall)
            {
                case 0:
                case 1:
                    shade = '.';
                break;
                case 2:
                    shade = '.';
                break;
                case 3:
                    shade = ':';
                break;
                case 4:
                    shade = '-';
                break;
                case 5:
                    shade = '=';
                break;
                case 6:
                    shade = '+';
                break;
                case 7:
                    shade = '*';
                break;
                case 8:
                    shade = '#';
                break;
                case 9:
                    shade = '%';
                break;
                default:
                    shade = '@';
            }

            for(int y = 0; y <= consoleHeight; y++)
            {
                if(y <= startCeiling)
                {
                    attron(COLOR_PAIR(1));
                    mvaddch(y, x, ' ');
                    attroff(COLOR_PAIR(1));
                }
                else if(y > startCeiling and y < startFloor)
                {
                    attron(COLOR_PAIR(2));
                    mvaddch(y, x, shade);
                    attroff(COLOR_PAIR(2));
                }
                else
                {
                    attron(COLOR_PAIR(3));
                    mvaddch(y, x, '.');
                    attroff(COLOR_PAIR(3));
                }
            }
        }
        
        for(int y = 0; y < size; y++)
        {
            for(int x = 0; x < size; x++)
            {
                mvaddch(y, x, maze[y][x]);
            }
        }

        mvaddch(playerY, playerX, 'P');

        refresh();
    }
}