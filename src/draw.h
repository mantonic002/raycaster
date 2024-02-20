#ifndef DRAW_H
#define DRAW_H

#include "./constants.h"

#define RAY_NUM 60
#define LINE_WIDTH (WINDOW_WIDTH/(RAY_NUM))
#define DEGREE 0.0174533 //degree in radians

void render(SDL_Renderer **renderer, Rectangle player, int **map, int *view);
void drawRays2D(SDL_Renderer **renderer, Rectangle player, int **map);
float dist(float ax, float ay, float bx, float by);
float fixAng(float a);

#endif // DRAW_H
