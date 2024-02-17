#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "./constants.h"
#include "./draw.h"
#include "./game.h"
#include "./maze.h"
#include "./game_loop.h"

int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // bool that is true if game is running
    bool game_is_running = initialize_window(&window, &renderer);

    // 2D array for the games map'
    // 0  empty space
    // 1 - 3  walls with different textures
    // 4  doors
    int **map = initialize_maze(BOARD_SIZE);
    generate_maze(map, BOARD_SIZE);

    Rectangle player;
    
    // variables for keeping fps consistent
    int last_frame_time = 0;
    float delta_time = 0.0f;

    // array for keeping track of which buttons are pressed
    bool keys[SDL_NUM_SCANCODES] = { false };

    setup(&player);
    
    while (game_is_running) {
        if (!process_input(keys, &player, map, &delta_time)) game_is_running = false;
        update(&last_frame_time, &delta_time);
        render(&renderer, player, map);
    }

    destroy_window(&window, &renderer);

    return 0;
}

