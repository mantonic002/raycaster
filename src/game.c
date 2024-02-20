#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "./constants.h"
#include "./game.h"

//-----function for changing game state based on players input-----
int process_input(bool *keys, Rectangle *player, int **map, float *delta_time, int *view) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return false;
            else if (event.key.keysym.sym == SDLK_f){
                if (*view == 0) {
                    *view = 1;
                } else {
                    *view = 0;
                }
                break;
            }
            else
                keys[event.key.keysym.scancode] = true;
            break;

        case SDL_KEYUP:
            keys[event.key.keysym.scancode] = false;
            break;

        default:
            break;
        }
    }

    // Update player positions based on key states
    if (keys[SDL_SCANCODE_A]) {
        player->a -= PLAYER_ROTATION_SPEED * *delta_time;
        if (player->a < 0) {
            player->a += 2*PI;
        }
        player->dx = cos(player->a) * PLAYER_LINE_LENGHT;
        player->dy = sin(player->a) * PLAYER_LINE_LENGHT;
    }

    if (keys[SDL_SCANCODE_D]) {
        player->a += PLAYER_ROTATION_SPEED * *delta_time;
        if (player->a > 2*PI){
            player->a -= 2*PI;
        }
        player->dx = cos(player->a) * PLAYER_LINE_LENGHT;
        player->dy = sin(player->a) * PLAYER_LINE_LENGHT;
    }

    //player offset depending on his position
    int xo = 0;
    if (player->dx < 0) xo = -5;  else xo = 5;
    
    int yo = 0;
    if (player->dy < 0) yo = -5;  else yo = 5;

    // players grid position
    int ipx = player->x/BOARD_SQUARE;
    int ipy = player->y/BOARD_SQUARE;

    int ipx_add_xo = (player->x + xo)/BOARD_SQUARE;
    int ipy_add_yo = (player->y + yo)/BOARD_SQUARE;

    int ipx_sub_xo = (player->x - xo)/BOARD_SQUARE;
    int ipy_sub_yo = (player->y - yo)/BOARD_SQUARE;


    if (keys[SDL_SCANCODE_S]) {    
        if (map[ipy][ipx_sub_xo] == 0) player->x -= PLAYER_SPEED * (*delta_time) * player->dx;
        if (map[ipy_sub_yo][ipx] == 0) player->y -= PLAYER_SPEED * (*delta_time) * player->dy;
    }

    if (keys[SDL_SCANCODE_W]) {
        if (map[ipy][ipx_add_xo] == 0) player->x += PLAYER_SPEED * *delta_time * player->dx;
        if (map[ipy_add_yo][ipx] == 0) player->y += PLAYER_SPEED * *delta_time * player->dy;
    }

    if (keys[SDL_SCANCODE_E]) {
        if (map[ipy][ipx_add_xo] == 4) map[ipy][ipx_add_xo] = 0;
        if (map[ipy_add_yo][ipx] == 4) map[ipy_add_yo][ipx] = 0;
    }

    return true;
}
