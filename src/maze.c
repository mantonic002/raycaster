#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "./constants.h"
#include "./maze.h"

typedef struct cell {
    int x;
    int y;
} cell;

cell stack[STACK_LENGTH];
int top = EMPTY;

bool push(cell value) {
    if (top >= STACK_LENGTH - 1) return false;
    top++;
    stack[top] = value;
    return true;
}

cell pop() {
    if (top == EMPTY) {
        cell empty_cell = { -1, -1 }; // Represents an empty cell
        return empty_cell;
    }
    top--;
    return stack[top + 1];
}

int** initialize_maze(int size) {
    // initialize a 2d array so that all the outside walls are 1, and inside part has a net of 1s and 0s
    int **maze = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        maze[i] = malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            if ((i % 2 != 1) || (j % 2 != 1)) maze[i][j] = 1;
            else maze[i][j] = 0;
        }
    }
    return maze;
}

bool is_valid(int x, int y, int size) {
    return (x >= 0 && x < size && y >= 0 && y < size);
}

bool generate_maze(int **maze, int size) {
    //starting coordinates
    cell start = {1, 1};

    maze[start.y][start.x] = -1;
    
    bool success = push(start);
    if (!success) return false;
    
    srand(time(NULL)); 

    while (top != EMPTY) {
        cell c = pop();
        //array of all neighbours
        cell neighbours_all[] = {{c.x, c.y+2}, {c.x+2, c.y}, {c.x, c.y-2}, {c.x-2, c.y}};
        
        //array of valid neighbours
        cell neighbours[4];
        int count = 0;
        //check if neighbours are all valid and add to neighbours
        for (int i = 0; i < 4; i++) {
            cell c = neighbours_all[i];
            if (is_valid(c.x, c.y, size) && maze[c.y][c.x] != (-1)) neighbours[count++] = c;
        }

        //if num of valid neighbours > 0
        if (count > 0) {
            //push current to stack
            success = push(c);
            if (!success) return false;

            //chose random neighbour
            int rand_num = rand() % (count);

            cell rn =  neighbours[rand_num];
            //remove the wall between current and chosen cell
            maze[(rn.y + c.y) >> 1][(rn.x + c.x) >> 1] = 0;
            //mark current as visited
            maze[rn.y][rn.x] = -1;

            success = push(rn);
            if (!success) return false;
        }
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++){
            if (maze[i][j] == -1) maze[i][j] = 0;
            else if (maze[i][j] == 1) maze[i][j] = 2;

        }
    }

    return true;
}