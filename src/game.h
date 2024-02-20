#ifndef GAME_H
#define GAME_H

#include "./constants.h"

#define PLAYER_SPEED 10 //optimal 25
#define PLAYER_ROTATION_SPEED 4 //optimal 

int process_input(bool *keys, Rectangle *player, int **map, float *delta_time, int *view);

#endif // GAME_H
