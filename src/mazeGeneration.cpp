#include <iostream>
#include <ncurses.h>
#include "mazeGeneration.h"
using namespace std;

struct CELL
{
	int y;
	int x;
	bool isVisited = false;
	bool isWall = false;
};

char **createMaze(CELL** maze, char **map, int *size)
{
	for(int i = 0; i < *size; i++)
	{
		for(int j = 0; j < *size; j++)
		{
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

void createWalls(CELL** maze, int *size, int* allCells)
{
  for(int y = 0; y < *size; y++)
	{
		for(int x = 0; x < *size; x++)
		{
			if((x % 2 == 0) or (y % 2 == 0))
			{
				maze[y][x].isWall = true;
			}
			else
			{
				++*allCells;
			}
		}
	}
}

void moveCharacter(CELL** maze, int* cordX, int* cordY, int dir, int* unvisitedCells)
{
  switch(dir)
		{
			case 0:
				if(maze[*cordY-2][*cordX].isVisited == false)
				{
					maze[*cordY-2][*cordX].isVisited = true;
					maze[*cordY-1][*cordX].isVisited = true;
					++*unvisitedCells;
				}
				*cordY -= 2;
				break;
			case 1:
				if(maze[*cordY][*cordX+2].isVisited == false)
				{
					maze[*cordY][*cordX+2].isVisited = true;
					maze[*cordY][*cordX+1].isVisited = true;
					++*unvisitedCells++;
				}
				*cordX += 2;
				break;
			case 2:
				if(maze[*cordY+2][*cordX].isVisited == false)
				{
					maze[*cordY+2][*cordX].isVisited = true;
					maze[*cordY+1][*cordX].isVisited = true;
					++*unvisitedCells++;
				}
				*cordY += 2;
				break;
			case 3:
				if(maze[*cordY][*cordX-2].isVisited == false)
				{
					maze[*cordY][*cordX-2].isVisited = true;
					maze[*cordY][*cordX-1].isVisited = true;
					++*unvisitedCells++;
				}
				*cordX -= 2;
				break;
		}
}

bool canMove(int dir, int size, int cordY, int cordX)
{
  return (dir == 0 && cordY == 1) or 
		 (dir == 2 && cordY == size-2) or 
		 (dir == 1 && cordX == size-2) or 
		 (dir == 3 && cordX == 1);
}

char **generateMaze(int mazeSize, int startY, int endY)
{
	echo();

	int allCells = 0;
	
	int cordX = 1;
	mazeSize = mazeSize * 2 + 1;

	CELL** mazeData = new CELL*[mazeSize];
	for(int i = 0; i < mazeSize; i++)
	{
		mazeData[i] = new CELL[mazeSize];
	}

	char **maze = new char*[mazeSize];
	for(int i = 0; i < mazeSize; i++)
	{
		maze[i] = new char[mazeSize];
	}

	mazeData[startY][0].isVisited = true;
	mazeData[endY][mazeSize - 1].isVisited = true;

	createWalls(mazeData, &mazeSize, &allCells);

	bool existUnvisitedCells = true;

	int unvisitedCells = 0;

	while(existUnvisitedCells)
	{
		if(unvisitedCells == allCells)
		{
			existUnvisitedCells = false;
		}
		int dir;
		do {
			dir = rand()%4;
		} while(canMove(dir, mazeSize, endY, cordX));
		
	moveCharacter(mazeData, &cordX, &endY, dir, &unvisitedCells);
	}

	createMaze(mazeData, maze, &mazeSize);

	return maze;
}