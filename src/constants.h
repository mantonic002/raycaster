#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H


#define BOARD_SIZE 21 // has to be odd
#define BOARD_SQUARE 25

#define WINDOW_HEIGHT (BOARD_SIZE * BOARD_SQUARE)
#define WINDOW_WIDTH WINDOW_HEIGHT

#define FPS 60

#define PLAYER_LINE_LENGHT 5

#define PI 3.1415926535
#define EPSILON 0.0001

typedef struct Rectangle {
    float x, y, dx, dy, a; // positon coordinates, offset coordinates and angle
    float width;
    float height;
} Rectangle;

#endif // GAME_CONSTANTS_H
