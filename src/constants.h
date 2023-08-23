#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#define WINDOW_HEIGHT 500
#define WINDOW_WIDTH 1000

#define BOARD_SIZE 8
#define BOARD_SQUARE (WINDOW_HEIGHT / BOARD_SIZE)

#define FPS 60

#define PLAYER_LINE_LENGHT 5

#define PI 3.1415926535

typedef struct Rectangle {
    float x, y, dx, dy, a; // positon coordinates, offset coordinates and angle
    float width;
    float height;
} Rectangle;

#endif // GAME_CONSTANTS_H
