#include <ncurses.h>
#include <math.h>
#include "rayCaster.h"

/**
 * @brief A function to check if player has won
 * 
 * @param currectPosX The current X position of the player
 * @param currentPosY The current Y position of the player
 * @param exitX The X exit coordinate of the maze
 * @param exitY The Y exit coordinate of the maze
 * @return true The player has exited the maze
 * @return false The player has not exited the maze
 */
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

/**
 * @brief Function handles user input
 * 
 * @param maze The generated maze
 * @param playerX The X coordinate of the player
 * @param playerY The Y coordinate to the player
 * @param playerAngle The angle player is looking at
 */
void handleInput(char** maze ,float& playerX, float& playerY, float& playerAngle)
{
    //Declate speed of turning and walking
    float walkSpeed = 0.1125f;
    float turnSpeed = 0.015f;

    //Switch that handles input
    switch(getch())
        {

            //Move player forwards if he is pressing w,W or KEY_UP
            case KEY_UP:
            case 'W':
            case 'w':
                playerX += sinf(playerAngle) * walkSpeed;
                playerY += cosf(playerAngle) * walkSpeed;
                //If player is trying to walk into a wall retrn them to start coordinate
                if(maze[(int)playerY][(int)playerX] == '#')
                {
                    playerX -= sinf(playerAngle) * walkSpeed;
                    playerY -= cosf(playerAngle) * walkSpeed;
                }
            break;

            //Move player backwards if he is pressing s,S or KEY_DOWN
            case KEY_DOWN:
            case 'S':
            case 's':
                playerX -= sinf(playerAngle) * walkSpeed;
                playerY -= cosf(playerAngle) * walkSpeed;
                //If player is trying to walk into a wall retrn them to start coordinate
                if(maze[(int)playerY][(int)playerX] == '#')
                {
                    playerX += sinf(playerAngle) * walkSpeed;
                    playerY += cosf(playerAngle) * walkSpeed;
                }
            break;

            //Turn player left if he is pressing a,A or KEY_LEFT
            case KEY_LEFT:
            case 'A':
            case 'a':
                playerAngle -= turnSpeed;
            break;

            //Turn player right if he is pressing d,D or KEY_RIGHT
            case KEY_RIGHT:
            case 'D':
            case 'd':
                playerAngle += turnSpeed;
            break;
        }
}

/**
 * @brief Function that calculates the distance to the wall
 * 
 * @param maze The generated maze
 * @param rayAngle The angle of the ray
 * @param distanceToWall The uncalculated distance to the wall
 * @param playerX The player X coordinate
 * @param playerY The player Y coordinate
 * @param size The size of the maze
 */
void findWallDistance(char **maze, float rayAngle, float& distanceToWall, float playerX, float playerY, int size)
{
    //Create a flag that checks if a wall is hit
    bool hitWall = false;

    //Find the X and Y direction of the rayAngle vector
    float eyeX = sinf(rayAngle);
    float eyeY = cosf(rayAngle);

    //Do this until a wall is hit and the distance to the wall is less than max render distance
    while(!hitWall && distanceToWall < 10)
    {
        //Increment the distance to the wall with a small size
        distanceToWall += 0.1f;

        //Find the end coordinates of the vector
        int testX = (int)(playerX + eyeX * distanceToWall);
        int testY = (int)(playerY + eyeY * distanceToWall);

        //If ray is OOB then the ray has hit
        if(testX < 0 || testX > size || testY < 0 || testY > size)
        {
            hitWall = true;

            //Set distance to wall to max render distance
            distanceToWall = 10;
        }
        else 
        {
            //Check if ray hit a wall
            if(maze[testY][testX] == '#')
            {
                hitWall = true;
            }
        }
    }
}

/**
 * @brief Calculate the ending coordinate of the ceiling
 * 
 * @param ceiling The ceiling coordinate
 * @param consoleHeight The height of the console
 * @param distanceToWall The calculated distance to the wall
 */
void calculateCeiling(int& ceiling ,int consoleHeight, float distanceToWall)
{
    ceiling = (float)(consoleHeight/2.0) - consoleHeight / distanceToWall;
}

/**
 * @brief Get the Shade object
 * 
 * @param shade Character representing a shade
 * @param distanceToWall Calculated distance to wall
 */
void getShade(char& shade, float distanceToWall)
{
    //Switch that determines the shade based on the distance to wall
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

/**
 * @brief Draws the raycasted image
 * 
 * @param x The column currently being printed
 * @param shade The shade of the wall based on distance to wall
 * @param consoleHeight Height of the console
 * @param endCeiling The ending coordinate of ceiling
 * @param startFloor The starting coordinate of floors
 * @param distanceToWall The distance to the wall
 */
void drawGame(int x, int shade, int consoleHeight, int endCeiling, int startFloor, int distanceToWall)
{
    //For loow that goes over every row of a pixel colunm
    for(int y = 0; y <= consoleHeight; y++)
    {
        //If Y is less than or equal to end of Ceiling - then that is a ceiling
        if(y <= endCeiling)
        {
            attron(COLOR_PAIR(SKY));
            mvaddch(y, x, ' ');
            attroff(COLOR_PAIR(SKY));
        }
        //If Y is between the start of the floor, the end of the ceiling and less than max render distance- then that is a wall
        else if(y > endCeiling and y < startFloor and distanceToWall != 10)
        {
            attron(COLOR_PAIR(WALLS));
            mvaddch(y, x, shade);
            attroff(COLOR_PAIR(WALLS));
        }
        //This is the floor
        else
        {
            attron(COLOR_PAIR(FLOOR));
            mvaddch(y, x, '.');
            attroff(COLOR_PAIR(FLOOR));
        }
    }
}

/**
 * @brief Function to draw the maze
 * 
 * @param maze The generated maze
 * @param size The size of the generated maze
 * @param playerY The Y coordinate of the player
 * @param playerX The X coordinate of the player
 */
void drawMap(char** maze, int size, int playerY, int playerX)
{
    //For() loop that loops over the whole map and prints it
    for(int y = 0; y < size; y++)
    {
        for(int x = 0; x < size; x++)
        {
            mvaddch(y, x, maze[y][x]);
        }
    }

    //Add the player based on his position
    mvaddch(playerY, playerX, 'P');
}

/**
 * @brief Raycaster. This function is used as a game loop
 * 
 * @param maze The generated maze
 * @param size The size of the generated maze
 * @param startY The starting coordinate of the player
 * @param endY The exit coordinate of the maze
 */
void rayCaster(char **maze,int size, int startY, int endY)
{
    //Get console dimenstions
    int consoleHeight = getmaxy(stdscr);
    int consoleWidth = getmaxx(stdscr);

    //Initial player position and angle
    float playerX = 1.0f;
    float playerY = startY;
    float playerAngle = 0.0f;

    //Field of view of the player
    float FOV = 3.14159 / 5.5;

    //Color pairs needed to draw objects
    start_color();
    init_pair(SKY, COLOR_BLUE, COLOR_BLUE);
    init_pair(WALLS, COLOR_BLACK, COLOR_WHITE);
    init_pair(FLOOR, COLOR_YELLOW, COLOR_YELLOW);

    //Main game loop
    while (!(checkIfWon(int(playerX), int(playerY), size-1, endY)))
    {
        //Handle player's input
        handleInput(maze, playerX, playerY, playerAngle);

        //Start casting rays
        for(int x = 0; x < consoleWidth; x++)
        {
            //Calculate the angle of ray at X screen column
            float rayAngle = (playerAngle - FOV/2.0) + ((float)x / (float)consoleWidth) * FOV;

            //Find distance to wall
            float distanceToWall = 0.0f;
            findWallDistance(maze, rayAngle, distanceToWall, playerX, playerY, size);

            //Find ceiling end coordinates and floor start coordinates
            int endCeiling;
            calculateCeiling(endCeiling, consoleHeight, distanceToWall);
            int startFloor = consoleHeight - endCeiling;

            //Get the sade of the walls based on distance from player to wall
            char shade = ' ';
            getShade(shade, distanceToWall);            

            //Draw the game
            drawGame(x, shade, consoleHeight, endCeiling, startFloor, distanceToWall);
        }
        
        //Draw the map
        drawMap(maze, size, playerY, playerX);
        refresh();
    }
}