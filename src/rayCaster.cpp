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

void handleInput(char** maze ,float& playerX, float& playerY, float& playerAngle)
{
    float speed = 1.5f;
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
}

void findWallDistance(char **maze, float rayAngle, float& distanceToWall, float playerX, float playerY, int size)
{
    bool hitWall = false;

    float eyeX = sinf(rayAngle);
    float eyeY = cosf(rayAngle);

    while(!hitWall && distanceToWall < 10)
    {
        distanceToWall += 0.1f;

        int testX = (int)(playerX + eyeX * distanceToWall);
        int testY = (int)(playerY + eyeY * distanceToWall);

        if(testX < 0 || testX > size || testY < 0 || testY > size)
        {
            hitWall = true;
            distanceToWall = 10;
        }
        else 
        {
            if(maze[testY][testX] == '#')
            {
                hitWall = true;
            }
        }
    }
}

void calculateCeiling(int& ceiling ,int consoleHeight, float distanceToWall)
{
    ceiling = (float)(consoleHeight/2.0) - consoleHeight / distanceToWall;
}

void getShade(char& shade, float distanceToWall)
{
    switch((int)distanceToWall)
    {
        case 0:
        case 1:
            shade = ' ';
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
}

void drawGame(int x, int shade, int consoleHeight, int startCeiling, int startFloor, int distanceToWall)
{
    for(int y = 0; y <= consoleHeight; y++)
    {
        if(y <= startCeiling)
        {
            attron(COLOR_PAIR(SKY));
            mvaddch(y, x, ' ');
            attroff(COLOR_PAIR(SKY));
        }
        else if(y > startCeiling and y < startFloor and distanceToWall != 10)
        {
            attron(COLOR_PAIR(WALLS));
            mvaddch(y, x, shade);
            attroff(COLOR_PAIR(WALLS));
        }
        else
        {
            attron(COLOR_PAIR(FLOOR));
            mvaddch(y, x, '.');
            attroff(COLOR_PAIR(FLOOR));
        }
    }
}

void drawMap(char** maze, int size, int playerY, int playerX)
{
    for(int y = 0; y < size; y++)
    {
        for(int x = 0; x < size; x++)
        {
            mvaddch(y, x, maze[y][x]);
        }
    }

    mvaddch(playerY, playerX, 'P');
}

void rayCaster(char **maze,int size, int startY, int endY)
{
    int consoleHeight = getmaxy(stdscr);
    int consoleWidth = getmaxx(stdscr);

    float playerX = 1.0f;
    float playerY = startY;
    float playerAngle = 0.0f;
    float FOV = 3.14159 / 5.5;

    start_color();
    init_pair(SKY, COLOR_BLUE, COLOR_BLUE);
    init_pair(WALLS, COLOR_BLACK, COLOR_WHITE);
    init_pair(FLOOR, COLOR_YELLOW, COLOR_YELLOW);

    while (!(checkIfWon(int(playerX), int(playerY), size-1, endY)))
    {
        handleInput(maze, playerX, playerY, playerAngle);
        for(int x = 0; x < consoleWidth; x++)
        {
            float rayAngle = (playerAngle - FOV/2.0) + ((float)x / (float)consoleWidth) * FOV;

            float distanceToWall = 0.0f;

            findWallDistance(maze, rayAngle, distanceToWall, playerX, playerY, size);

            int startCeiling;
            calculateCeiling(startCeiling, consoleHeight, distanceToWall);
            int startFloor = consoleHeight - startCeiling;

            char shade = ' ';
            
            getShade(shade, distanceToWall);            

            drawGame(x, shade, consoleHeight, startCeiling, startFloor, distanceToWall);
        }
        
        drawMap(maze, size, playerY, playerX);

        refresh();
    }
}