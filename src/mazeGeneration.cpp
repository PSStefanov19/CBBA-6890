#include <iostream>
#include <time.h>
#include "mazeGeneration.h"
using namespace std;

struct CELL
{
	int y;
	int x;
	bool isVisited = false;
	bool isWall = false;
};

void printMaze(CELL** maze, int size, char pathSymbol)
{
  for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			if(maze[i][j].isVisited == true)
			{
				cout << pathSymbol << " ";
			}
			else
			{
				cout << "# ";
			}
		}
		cout<<endl;
	}
}

void createWalls(CELL** maze, int size, int* allCells)
{
  for(int y = 0; y < size; y++)
	{
		for(int x = 0; x < size; x++)
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

bool isAbleToMove(int dir, int size, int cordY, int cordX)
{
  return (dir == 0 && cordY == 1) or 
			   (dir == 2 && cordY == size-2) or 
			   (dir == 1 && cordX == size-2) or 
			   (dir == 3 && cordX == 1);
}

void generateMaze()
{
	srand(time(NULL));
	int cordY, cordX = 1;
	int allCells = 0;
  char pathSymbol = ' ';

  int size;
  
	cin >> size;

  cordY = (rand() % size) * 2 + 1;

  int exitY = (rand() % size) * 2 + 1;

	size = size * 2 + 1;

  CELL** maze = new CELL*[size];
  for(int i = 0; i < size; i++)
  {
    maze[i] = new CELL[size];
  }

  maze[cordY][cordX - 1].isVisited = true;
  maze[exitY][size - 1].isVisited = true;

	createWalls(maze, size, &allCells);

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
		} while(isAbleToMove(dir, size, cordY, cordX));
		
    moveCharacter(maze, &cordX, &cordY, dir, &unvisitedCells);
	}

  printMaze(maze, size, pathSymbol);
}