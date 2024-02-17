#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "./constants.h"
#include "./game_loop.h"

//----function for initializing sdl window and renderer------
int initialize_window(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    *window = SDL_CreateWindow (
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if (!*window) {
        fprintf(stderr, "Error initializing SDL window.\n");
        return false;
    }

    *renderer = SDL_CreateRenderer(
        *window,
        -1,
        0
    );

    if (!*renderer) {
        fprintf(stderr, "Error initializing SDL renderer.\n");
        return false;
    }

    return true;
}

void setup(Rectangle *player) {
    player->x = BOARD_SQUARE * 1.5;
    player->y = BOARD_SQUARE * 1.5;
    player->a = 0;
    player->width = 10;
    player->height = 10;
    player->dx = cos(player->a) * PLAYER_LINE_LENGHT;
    player->dy = sin(player->a) * PLAYER_LINE_LENGHT;
}



//----function that keeps consistent frame rate----
void update(int *last_frame_time, float *delta_time) {

    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - *last_frame_time);
    
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    // delta time factor in seconds
    *delta_time = (SDL_GetTicks() - *last_frame_time) / 1000.0f;

    *last_frame_time = SDL_GetTicks();
    
}

//-----function that destroys the window and quits the game--------
void destroy_window(SDL_Window **window, SDL_Renderer **renderer) {
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);
    SDL_Quit();
}