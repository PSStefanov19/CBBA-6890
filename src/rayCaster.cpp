#include <ncurses.h>
#include <math.h>

void rayCaster(char **maze,int size)
{
    int consoleHeight = getmaxy(stdscr);
    int consoleWidth = getmaxx(stdscr);
    int mapWidth = size;
    int mapHeight = size;

    float playerX = 1.0f;
    float playerY = 1.0f;
    float playerAngle = 0.0f;
    float FOV = 3.14159f / 4.0f;
    float maxRenderDistance = 10.0f;
    float speed = 1.0f;

    while (true)
    {
        char ch;
        switch(ch = getch())
        {
            case 'w':
                playerX += sinf(playerAngle) * speed * 0.075;
                playerY += cosf(playerAngle) * speed * 0.075;
                if(maze[(int)playerY][(int)playerX] == '#')
                {
                    playerX -= sinf(playerAngle) * speed * 0.075;
                    playerY -= cosf(playerAngle) * speed * 0.075;
                }
            break;
            case 's':
                playerX -= sinf(playerAngle) * speed * 0.075;
                playerY -= cosf(playerAngle) * speed * 0.075;
                if(maze[(int)playerY][(int)playerX] == '#')
                {
                    playerX += sinf(playerAngle) * speed * 0.075;
                    playerY += cosf(playerAngle) * speed * 0.075;
                }
            break;
            case 'a':
                playerAngle -= speed * 0.01;
            break;
            case 'd':
                playerAngle += speed * 0.01;
            break;
        }
        for(int x = 0; x < consoleWidth; x++)
        {
            
            float rayAngle = (playerAngle - FOV/2.0f) + ((float)x / (float)consoleWidth) * FOV;

            float stepSize = 0.01f;
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
            int startCeiling = (consoleHeight / 2) - (consoleHeight / distanceToWall);
            int startFloor = consoleHeight - startCeiling;

            char shade = ' ';


            // .:-=+*#%@
            switch((int)distanceToWall)
            {
                case 0:
                case 1:
                    shade = '@';
                    break;
                case 2:
                    shade = '%';
                    break;
                case 3:
                    shade = '#';
                    break;
                case 4:
                    shade = '*';
                    break;
                case 5:
                    shade = '+';
                    break;
                case 6:
                    shade = '=';
                    break;
                case 7:
                    shade = '-';
                    break;
                case 8:
                    shade = ':';
                    break;
                case 9:
                    shade = '.';
                    break;
                case 10:
                    shade = ' ';
                    break;
            }	

            for(int y = 0; y <= consoleHeight; y++)
            {
                if(y <= startCeiling)
                {
                    mvaddch(y, x, ' ');
                }
                else if(y > startCeiling and y < startFloor)
                {
                    mvaddch(y, x, shade);
                }
                else
                {
                    mvaddch(y, x, '.');
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
