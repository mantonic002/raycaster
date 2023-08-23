#ifndef GAME_H
#define GAME_H

#include "./constants.h"

#define PLAYER_SPEED 25 //optimal 25
#define PLAYER_ROTATION_SPEED 2 //optimal 2

int process_input(bool *keys, Rectangle *player, int map[BOARD_SIZE][BOARD_SIZE], float *delta_time);

#endif // GAME_H
