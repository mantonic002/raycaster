#include <limits.h>

#ifndef MAZE_H
#define MAZE_H

#define STACK_LENGTH 500
#define EMPTY (-1)
#define STACK_EMPTY INT_MIN

int ** initialize_maze(int size);
bool generate_maze(int **maze, int size);

#endif