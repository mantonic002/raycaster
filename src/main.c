#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "./constants.h"
#include "./draw.h"
#include "./game.h"
#include "./maze.h"
#include "./game_loop.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool game_is_running;
int **map;

Rectangle player;

// variables for keeping fps consistent
int last_frame_time = 0;
float delta_time = 0.0f;

// if view = 0, show first person view
// if view = 1, show top down view
int view = 0;

// array for keeping track of which buttons are pressed
bool keys[SDL_NUM_SCANCODES] = { false };


static void mainloop(void) {
    if (!game_is_running) {
        destroy_window(&window, &renderer);
        #ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();  /* this should "kill" the app. */
        #else
        exit(0);
        #endif
    }

    if(((int)(player.x/BOARD_SQUARE) == (BOARD_SIZE - 2)) && ((int)(player.y/BOARD_SQUARE) == (BOARD_SIZE - 2))) {
        map = initialize_maze(BOARD_SIZE);
        generate_maze(map, BOARD_SIZE);  

        setup(&player);
    }
    if (!process_input(keys, &player, map, &delta_time, &view)) game_is_running = false;
    update(&last_frame_time, &delta_time);
    render(&renderer, player, map, &view);
}

int main(void) {
    // bool that is true if game is running
    game_is_running = initialize_window(&window, &renderer);

    // 2D array for the games map'
    // 0  empty space
    // 1 - 3  walls with different textures
    // 4  doors
    map = initialize_maze(BOARD_SIZE);
    generate_maze(map, BOARD_SIZE);  

    setup(&player);
    
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop, 0, 1);
    #else
    while (1) { mainloop(); }
    #endif

    return 0;
}

