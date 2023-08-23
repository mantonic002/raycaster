#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#define FRAME_TARGET_TIME (1000 / FPS)

int initialize_window(SDL_Window **window, SDL_Renderer **renderer);
void setup(Rectangle *player);
void update(int *last_frame_time, float *delta_time);
void destroy_window(SDL_Window **window, SDL_Renderer **renderer);

#endif // GAME_LOOP_H