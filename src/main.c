#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "./constants.h"
#include <math.h>


typedef struct Rectangle {
    float x, y, dx, dy, a; // positon coordinates, offset coordinates and angle
    float width;
    float height;
} Rectangle;


int initialize_window(SDL_Window **window, SDL_Renderer **renderer);
void setup(Rectangle player);
int process_input(bool *keys, Rectangle player, float *delta_time);
void update(int *last_frame_time, float *delta_time);
void render(SDL_Renderer **renderer, Rectangle player);
void destroy_window(SDL_Window *window, SDL_Renderer *renderer);
void drawRays2D(SDL_Renderer **renderer, Rectangle player);
float dist(float ax, float ay, float bx, float by);
float fixAng(float a);


int map[BOARD_SIZE][BOARD_SIZE] = {
    {1, 1, 1, 1, 4, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {4, 0, 0, 0, 0, 0, 0, 4},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 4, 1, 1, 1},
    
};

int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    bool game_is_running = initialize_window(&window, &renderer);

    Rectangle player;
    
    int last_frame_time = 0;
    float delta_time = 0.0f;

    bool keys[SDL_NUM_SCANCODES] = { false };

    setup(player);
    
    while (game_is_running) {
        if (!process_input(keys, player, &delta_time)) game_is_running = false;
        update(&last_frame_time, &delta_time);
        render(&renderer, player);
    }

    destroy_window(window, renderer);

    return 0;
}

//----function for initializing sdl window and renderer------
int initialize_window(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    *window = SDL_CreateWindow(
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

void setup(Rectangle player) {
    player.x = WINDOW_WIDTH / 4;
    player.y = WINDOW_HEIGHT / 2;
    player.a = 0;
    player.width = 10;
    player.height = 10;
    player.dx = cos(player.a) * PLAYER_LINE_LENGHT;
    player.dy = sin(player.a) * PLAYER_LINE_LENGHT;
}

//-----function for changing game state based on players input-----
int process_input(bool *keys, Rectangle player, float *delta_time) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return false;
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
        player.a -= PLAYER_ROTATION_SPEED * *delta_time;
        if (player.a < 0) {
            player.a += 2*PI;
        }
        player.dx = cos(player.a) * PLAYER_LINE_LENGHT;
        player.dy = sin(player.a) * PLAYER_LINE_LENGHT;
    }

    if (keys[SDL_SCANCODE_D]) {
        player.a += PLAYER_ROTATION_SPEED * *delta_time;
        if (player.a > 2*PI){
            player.a -= 2*PI;
        }
        player.dx = cos(player.a) * PLAYER_LINE_LENGHT;
        player.dy = sin(player.a) * PLAYER_LINE_LENGHT;
    }

    //player offset depending on his position
    int xo = 0;
    if (player.dx < 0) xo = -20;  else xo = 20;
    
    int yo = 0;
    if (player.dy < 0) yo = -20;  else yo = 20;

    // players grid position
    int ipx = player.x/BOARD_SQUARE;
    int ipy = player.y/BOARD_SQUARE;

    int ipx_add_xo = (player.x + xo)/BOARD_SQUARE;
    int ipy_add_yo = (player.y + yo)/BOARD_SQUARE;

    int ipx_sub_xo = (player.x - xo)/BOARD_SQUARE;
    int ipy_sub_yo = (player.y - yo)/BOARD_SQUARE;


    if (keys[SDL_SCANCODE_S]) {    
        if (map[ipy][ipx_sub_xo] == 0) player.x -= PLAYER_SPEED * (*delta_time) * player.dx;
        if (map[ipy_sub_yo][ipx] == 0) player.y -= PLAYER_SPEED * (*delta_time) * player.dy;
    }

    if (keys[SDL_SCANCODE_W]) {
        if (map[ipy][ipx_add_xo] == 0) player.x += PLAYER_SPEED * *delta_time * player.dx;
        if (map[ipy_add_yo][ipx] == 0) player.y += PLAYER_SPEED * *delta_time * player.dy;
    }

    if (keys[SDL_SCANCODE_E]) {
        if (map[ipy][ipx_add_xo] == 4) map[ipy][ipx_add_xo] = 0;
        if (map[ipy_add_yo][ipx] == 4) map[ipy_add_yo][ipx] = 0;
    }

    return true;
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

//----function for rendering the 2D topdown game-----
void render(SDL_Renderer **renderer, Rectangle player) {
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
    SDL_RenderClear(*renderer);

    //rendering ceeling
    SDL_Rect ceel = {
        WINDOW_WIDTH/2 + 1,
        0,
        WINDOW_WIDTH/2 - 20,
        WINDOW_HEIGHT/2
    };

    SDL_SetRenderDrawColor(*renderer, 128, 128, 128, 255);
    SDL_RenderFillRect(*renderer, &ceel);

    //rendering floor
    SDL_Rect floor = {
        WINDOW_WIDTH/2 + 1,
        WINDOW_HEIGHT/2,
        WINDOW_WIDTH/2 - 20,
        WINDOW_HEIGHT/2
    };

    SDL_SetRenderDrawColor(*renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(*renderer, &floor);

    //-----draw walls
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {

            if (map[i][j] > 0) {
                SDL_SetRenderDrawColor(*renderer, 255, 255,255, 255);
 
                SDL_Rect wall = {
                    j*BOARD_SQUARE,
                    i*BOARD_SQUARE ,
                    BOARD_SQUARE - 1,
                    BOARD_SQUARE - 1
                };
                SDL_RenderFillRect(*renderer, &wall);
            }

        }
    }

    // draw player
    int x = player.x;
    int y = player.y;

    int x2 = player.x + player.dx*5;
    int y2 = player.y + player.dy*5;

    drawRays2D(renderer, player);

    SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);

    SDL_RenderDrawLine(*renderer, x, y, x2, y2);


    SDL_RenderPresent(*renderer);
}

//-----helper function that calculates distance between two points------
float dist(float ax, float ay, float bx, float by) {
    float x = bx - ax;
    float y = by - ay;
    return ( sqrt( y * y + x * x) );
}

//-----helper function that fixes the range of angle if it exeeds------
float fixAng(float a) { 
    if(a >= 2*PI) a -= 2*PI; 
    if(a < 0) a += 2*PI;
    return a;
}

//-----function that calculates and casts rays, and draws walls------
void drawRays2D(SDL_Renderer **renderer, Rectangle player) {
    // raynum
    // mx, my - position of rays end in map
    // dof - depth of field
    int r, mx, my, dof;
    //rx. ry - first horizontal line hits
    //ra - ray angle
    //xo, yo - x and y offsets
    //disT - final distance
    float rx, ry, ra, xo, yo, disT;

    ra = fixAng(player.a - (DEGREE * 30));

    for(r = 0; r < RAY_NUM; r++) {
        int vmt = 0, hmt = 0; // vertical and horizontal map texture num

        // Check horizontal lines
        dof = 0;
        float hx = player.x, hy = player.y, disH = 100000;
        float aTan = -1/tan(ra);

        if(ra > PI) { //looking up
            ry = (((int)player.y/BOARD_SQUARE)*BOARD_SQUARE)-0.0001;
            rx = (player.y - ry) * aTan + player.x;
            yo = -BOARD_SQUARE;
            xo = -yo * aTan;
        }
        else if(ra < PI) { //looking down
            ry = (((int)player.y/BOARD_SQUARE) + 1) * BOARD_SQUARE;
            rx = (player.y - ry) * aTan + player.x;
            yo = BOARD_SQUARE;
            xo = -yo * aTan;
        }
        else if (ra == 0 || ra == PI) { //looking straight left of right
            rx = player.x;
            ry = player.y;
            dof = 8;
        }
        while(dof < 8) {
            mx = (int) (rx)/BOARD_SQUARE;
            my = (int) (ry)/BOARD_SQUARE;

            if (mx < BOARD_SIZE && my < BOARD_SIZE && mx >= 0 && my >= 0) {
                if (map[my][mx] > 0) { //hit wall
                    hx = rx;
                    hy = ry;
                    disH = dist(player.x, player.y, hx, hy);
                    dof = 8;
                    hmt = map[my][mx] - 1;
                } else { // next line
                    rx += xo; 
                    ry += yo;
                    dof += 1;
                }
            } else dof = 8;
        }

        // Check vertical lines
        dof = 0;
        float vx = player.x, vy = player.y, disV = 100000;
        float nTan = -tan(ra);

        if(ra > PI/2 && ra < 3 * PI/2) { //looking left
            rx = (((int)player.x/BOARD_SQUARE)*BOARD_SQUARE)-0.0001;
            ry = (player.x - rx) * nTan + player.y;
            xo = -BOARD_SQUARE;
            yo = -xo * nTan;
        }
        else if(ra < PI/2 || ra > 3 * PI/2) { //lookng right
            rx = (((int)player.x/BOARD_SQUARE + 1)*BOARD_SQUARE);
            ry = (player.x - rx) * nTan + player.y;
            xo = BOARD_SQUARE;
            yo = -xo * nTan;
        }
        else if (ra == PI/2 || ra == 3 * PI/2) { //looking straight up of down
            rx = player.x;
            ry = player.y;
            dof = 8;
        }

        while(dof < 8) {
            mx = (int) (rx)/BOARD_SQUARE;
            my = (int) (ry)/BOARD_SQUARE;

            if (mx < BOARD_SIZE && my < BOARD_SIZE && mx >= 0 && my >= 0) {
                if (map[my][mx] > 0) { //hit wall
                    vx = rx;
                    vy = ry;
                    disV = dist(player.x, player.y, vx, vy);
                    dof = 8;
                    vmt = map[my][mx] - 1;
                } else { // next line
                    rx += xo; 
                    ry += yo;
                    dof += 1;
                }
            } else dof = 8;
        }

        float shade;

        //Check if horizontal or vertical is shorter, and draw the shorter one
        if (disV < disH) {
            hmt = vmt;
            rx = vx;
            ry = vy;
            disT = disV;
            shade = 1;
        } else if (disV >= disH) {
            rx = hx;
            ry = hy;
            disT = disH;
            shade = 0.5;
        }

        SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(*renderer, player.x, player.y, rx, ry);

        //Draw 3D walls
        float ca = fixAng(player.a - ra);

        disT = disT * cos(ca); 
        float lineH = ((BOARD_SIZE * WINDOW_HEIGHT) / disT) * 3;

        float ty_step = 32/(float)lineH;
        float ty_off = 0;

        if (lineH > WINDOW_HEIGHT) {
            ty_off = (lineH - WINDOW_HEIGHT) / 2.0;
            lineH = WINDOW_HEIGHT;
        }
        float offset = WINDOW_HEIGHT / 2 - lineH/2;

        // drawing textures
        int y;
        float ty = ty_off * ty_step + hmt*32;

        float tx;
        if (shade == 1) {
            tx = (int)(ry / ((float) BOARD_SQUARE / 32)) % 32;
            if (ra > PI/2 && ra < 3*PI/2) tx = 31 - tx;
        } 
        else {
            tx = (int)(rx / ((float) BOARD_SQUARE / 32)) % 32;
            if (ra < PI)  tx = 31 - tx;        
        }
        
        for (y = 0 ; y < lineH ; y++) {

            float c = All_Textures[(int)(ty)*32 + (int)(tx)]*255*shade;
            SDL_SetRenderDrawColor(*renderer, c, c, c, 255);
            SDL_Rect wall = {
                r * LINE_WIDTH + WINDOW_WIDTH/2,
                y + offset,
                LINE_WIDTH,
                1
                };
            SDL_RenderFillRect(*renderer, &wall);
            ty+=ty_step;
        }

        ra = fixAng(ra + DEGREE);
    }
}

//-----function that destroys the window and quits the game--------
void destroy_window(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}