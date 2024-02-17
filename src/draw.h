#ifndef DRAW_H
#define DRAW_H

#include "./constants.h"

#define RAY_NUM 60
#define LINE_WIDTH (WINDOW_WIDTH/(2*RAY_NUM))
#define DEGREE 0.0174533 //degree in radians

void render(SDL_Renderer **renderer, Rectangle player, int map[BOARD_SIZE][BOARD_SIZE]);
void drawRays2D(SDL_Renderer **renderer, Rectangle player, int map[BOARD_SIZE][BOARD_SIZE]);
float dist(float ax, float ay, float bx, float by);
float fixAng(float a);

#endif // DRAW_H
