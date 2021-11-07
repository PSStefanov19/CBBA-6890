#include <iostream>
#include <ncurses.h>
#include "mazeGeneration.h"
using namespace std;

/**
 * @brief Structure that holds data for a single cell of the maze
 * 
 */
struct CELL
{
	int y;
	int x;
	bool isVisited = false;
	bool isWall = false;
};

/**
 * @brief Transform the maze from a CELL maze to a char map
 * 
 * @param maze The maze made up of CELL structures
 * @param map The map made of characters
 * @param size The size of the maze
 * @return char** return the maze
 */
char **createMaze(CELL** maze, char **map, int *size)
{
	//Turn CELL maze into a char map
	for(int i = 0; i < *size; i++)
	{
		for(int j = 0; j < *size; j++)
		{
			//Check if cell is visited
			if(maze[i][j].isVisited == true)
			{
				map[i][j] = ' ';
			}
			else
			{
				map[i][j] = '#';
			}
		}
	}
	return map;
}

/**
 * @brief Create walls
 * 
 * @param maze The maze of cell data
 * @param size The size of the maze
 * @param allCells Number of all cells that can be visited
 */
void createWalls(CELL** maze, int &size, int& allCells)
{
	//Loop throught the whole maze
	for(int y = 0; y < size; y++)
	{
		for(int x = 0; x < size; x++)
		{
			//Check if the X or Y is a wall
			if((x % 2 == 0) or (y % 2 == 0))
			{
				//Set the isWall flag to true
				maze[y][x].isWall = true;
			}
			else
			{
				//Add a cell to cells that can be visited
				++allCells;
			}
		}
	}
}

/**
 * @brief Function that moves the current cell to given direction
 * 
 * @param maze The array with the date from the maze
 * @param cordX THe X coordinate of the current cell
 * @param cordY The Y coordinate of the current cell
 * @param dir The direction a cell should be moced to
 * @param visitedCells Number of visted cells
 */
void moveCharacter(CELL** maze, int &cordX, int &cordY, int dir, int &visitedCells)
{
	//Switch to move current cell to given direction
	switch(dir)
	{
		//Move current cell up
		case 0:
			//Check if upward cell is visited
			if(maze[cordY-2][cordX].isVisited == false)
			{
				maze[cordY-2][cordX].isVisited = true;
				maze[cordY-1][cordX].isVisited = true;
				++visitedCells;
			}
			cordY -= 2;
			break;
		//Move current cell left
		case 1:
			//Check if the cell to the left is visited
			if(maze[cordY][cordX+2].isVisited == false)
			{
				maze[cordY][cordX+2].isVisited = true;
				maze[cordY][cordX+1].isVisited = true;
				++visitedCells;
			}
			cordX += 2;
			break;
		//Move current cell down
		case 2:
			//Check if the downward cell is visited
			if(maze[cordY+2][cordX].isVisited == false)
			{
				maze[cordY+2][cordX].isVisited = true;
				maze[cordY+1][cordX].isVisited = true;
				++visitedCells;
			}
			cordY += 2;
			break;
		//Move current cell right
		case 3:
			//Check if the cell to the right is visted
			if(maze[cordY][cordX-2].isVisited == false)
			{
				maze[cordY][cordX-2].isVisited = true;
				maze[cordY][cordX-1].isVisited = true;
				++visitedCells;
			}
			cordX -= 2;
			break;
	}
}

/**
 * @brief A function to check whether the current cell can be moved to a certain direction
 * 
 * @param dir Direction the cell is moved to
 * @param size The size of the maze
 * @param cordY The Y coordinate of the current cell
 * @param cordX The X coordinate of the current cell
 * @return true Current cell cannot be moved to direction
 * @return false Current cell can be moved to direction
 */
bool canNotMove(int dir, int size, int cordY, int cordX)
{
  return (dir == 0 && cordY == 1) or 
		 (dir == 2 && cordY == size-2) or 
		 (dir == 1 && cordX == size-2) or 
		 (dir == 3 && cordX == 1);
}

char **generateMaze(int mazeSize, int startY, int endY)
{
	//A variable holding all cells that can be visited
	int allCells = 0;
	
	//The starting coordinates of the maze generation
	int cordX = 1;
	int cordY = 1;

	//The maze with added walls
	mazeSize = mazeSize * 2 + 1;

	//An array of CELLS that holds all the data to the maze
	CELL** mazeData = new CELL*[mazeSize];
	for(int i = 0; i < mazeSize; i++)
	{
		mazeData[i] = new CELL[mazeSize];
	}

	//A character array that will hold the finished maze
	char **maze = new char*[mazeSize];
	for(int i = 0; i < mazeSize; i++)
	{
		maze[i] = new char[mazeSize];
	}

	//Make a random cell from the first column the entrance to the maze
	mazeData[startY][0].isVisited = true;

	//Make a random cell from the last column the exit of the maze
	mazeData[endY][mazeSize - 1].isVisited = true;

	//Change the isWall flag on walls of the maze
	createWalls(mazeData, mazeSize, allCells);

	//Flag for unvisited cells
	bool existUnvisitedCells = true;

	//A variable holding all cells that holds all visited cells
	int visitedCells = 0;

	//While unvisited cells exist
	while(existUnvisitedCells)
	{
		//Check if the visited cells are equal to all visitable cells
		if(visitedCells == allCells)
		{
			existUnvisitedCells = false;
		}
		//Choose a direction to move current cell to
		int dir;
		do {
			dir = rand()%4;
		} while(canNotMove(dir, mazeSize, cordY, cordX));
		
		//Move cell to chosen direction
		moveCharacter(mazeData, cordX, cordY, dir, visitedCells);
	}

	//Transfer data from mazeData to the character array
	createMaze(mazeData, maze, &mazeSize);

	//Return the character array
	return maze;
}