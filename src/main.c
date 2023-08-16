#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "./constants.h"
#include <math.h>

int initialize_window(void);
void setup(void);
void process_input(void);
void update(void);
void render(void);
void destroy_window(void);

bool game_is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;
float delta_time = 0.0f;

bool keys[SDL_NUM_SCANCODES] = { false };

int map[BOARD_SIZE][BOARD_SIZE] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    
};

struct rectangle {
    float x, y, dx, dy, a;

    float width;
    float height;
} player;


float dist(float ax, float ay, float bx, float by) {
    float x = bx - ax;
    float y = by - ay;
    return ( sqrt( y * y + x * x) );
}

void drawRays2D() 
{
    // raynum
    // mx, my - position of rays end in map
    //dof - depth of field
    int r, mx, my, dof;
    //rx. ry - first horizontal line hits
    //ra - ray angle
    //xo, yo - x and y offsets
    //disT - final distance
    float rx, ry, ra, xo, yo, disT;

    ra = player.a - DEGREE *30;

    if(ra < 0) { ra += 2*PI;}
    if(ra > 2*PI) { ra -= 2*PI;}

    for(r = 0; r < RAY_NUM; r++)
    {
        // Check horizontal lines
        dof = 0;
        float hx = player.x, hy = player.y, disH;
        float aTan = -1/tan(ra);

        if(ra > PI) //looking up
        {
            ry = (((int)player.y/BOARD_SQUARE)*BOARD_SQUARE)-0.0001;
            rx = (player.y - ry) * aTan + player.x;
            yo = -BOARD_SQUARE;
            xo = -yo * aTan;
        }
        else if(ra < PI) //looking down
        {
            ry = (((int)player.y/BOARD_SQUARE) + 1) * BOARD_SQUARE;
            rx = (player.y - ry) * aTan + player.x;
            yo = BOARD_SQUARE;
            xo = -yo * aTan;
        }
        else if (ra == 0 || ra == PI) //looking straight left of right
        {
            rx = player.x;
            ry = player.y;
            dof = 8;
        }
        while(dof < 8)
        {
            mx = (int) (rx)/BOARD_SQUARE;
            my = (int) (ry)/BOARD_SQUARE;

            if (mx < BOARD_SQUARE && my < BOARD_SQUARE && mx >= 0 && my >= 0)
            {
                if (map[my][mx] == 1) //hit wall
                {
                    hx = rx;
                    hy = ry;
                    disH = dist(player.x, player.y, hx, hy);
                    dof = 8;
                } else // next line
                { 
                    rx += xo; 
                    ry += yo;
                    dof += 1;
                }
            } else dof = 8;
        }

        // Check vertical lines
        dof = 0;
        float vx = player.x, vy = player.y, disV;
        float nTan = -tan(ra);

        if(ra > PI/2 && ra < 3 * PI/2) //looking left
        {
            rx = (((int)player.x/BOARD_SQUARE)*BOARD_SQUARE)-0.0001;
            ry = (player.x - rx) * nTan + player.y;
            xo = -BOARD_SQUARE;
            yo = -xo * nTan;
        }
        else if(ra < PI/2 || ra > 3 * PI/2) //lookng right
        {
            rx = (((int)player.x/BOARD_SQUARE + 1)*BOARD_SQUARE);
            ry = (player.x - rx) * nTan + player.y;
            xo = BOARD_SQUARE;
            yo = -xo * nTan;
        }
        else if (ra == PI/2 || ra == 3 * PI/2) //looking straight up of down
        {
            rx = player.x;
            ry = player.y;
            dof = 8;
        }

        while(dof < 8)
        {
            mx = (int) (rx)/BOARD_SQUARE;
            my = (int) (ry)/BOARD_SQUARE;
            if (map[my][mx] == 1) //hit wall
            {
                vx = rx;
                vy = ry;
                disV = dist(player.x, player.y, vx, vy);
                dof = 8;
            } else // next line
            { 
                rx += xo; 
                ry += yo;
                dof += 1;
            }
        }

        //Check if horizontal or vertical is shorter, and draw the shorter one
        if (disV < disH) {
            rx = vx;
            ry = vy;
            disT = disV;
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        } else if (disV >= disH) {
            rx = hx;
            ry = hy;
            disT = disH;
            SDL_SetRenderDrawColor(renderer, 50, 50, 255, 255);
        }


        SDL_RenderDrawLine(renderer, player.x, player.y, rx, ry);

        //Draw 3D walls
        float ca = player.a - ra;
        if (ca < 0) ca += 2*PI; 
        if (ca > 2*PI) ca -= 2*PI;
        disT = disT * cos(ca); 
        float lineH = (BOARD_SIZE * WINDOW_HEIGHT) / disT;
        //if (lineH > WINDOW_HEIGHT/2) lineH = WINDOW_HEIGHT/2;

        float offset = WINDOW_HEIGHT/2 - lineH/2;

        SDL_Rect wall = {
            r * LINE_WIDTH + WINDOW_WIDTH/2,
            offset,
            LINE_WIDTH,
            lineH
            };
        SDL_RenderFillRect(renderer, &wall);

        ra += DEGREE;
        if(ra < 0) { ra += 2*PI;}
        if(ra > 2*PI) { ra -= 2*PI;}
    }
}

int main() {
    game_is_running = initialize_window();

    setup();
    
    while (game_is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}


int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    
    if (TTF_Init() != 0) {
        fprintf(stderr, "Error initializing SDL_ttf.\n");
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        fprintf(stderr, "Error initializing SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        0
    );
    if (!renderer) {
        fprintf(stderr, "Error initializing SDL renderer.\n");
        return false;
    }

    return true;
}

void setup() {
    player.x = WINDOW_WIDTH / 4;
    player.y = WINDOW_HEIGHT / 2;
    player.width = 10;
    player.height = 10;
}

void process_input() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            game_is_running = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                game_is_running = false;
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
    if (keys[SDL_SCANCODE_A])
    {
        player.a -= 0.03;
        if (player.a < 0) {
            player.a += 2*PI;
        }
        player.dx = cos(player.a) * 5;
        player.dy = sin(player.a) * 5;
    }

    if (keys[SDL_SCANCODE_D])
    {
        player.a += 0.03;
        if (player.a > 2*PI){
            player.a -= 2*PI;
        }
        player.dx = cos(player.a) * 5;
        player.dy = sin(player.a) * 5;
    }
    if (keys[SDL_SCANCODE_S])
    {    
        player.y -= PLAYER_SPEED * delta_time * player.dy;
        player.x -= PLAYER_SPEED * delta_time * player.dx;
    }

    if (keys[SDL_SCANCODE_W])
    {
        player.y += PLAYER_SPEED * delta_time * player.dy;
        player.x += PLAYER_SPEED * delta_time * player.dx;
    }
    }



void update() {

    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    // delta time factor in seconds
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();
    
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {

            if (map[i][j] == 1) 
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                SDL_Rect wall = {
                    j*BOARD_SQUARE,
                    i*BOARD_SQUARE ,
                    BOARD_SQUARE - 1,
                    BOARD_SQUARE - 1
                };
                SDL_RenderFillRect(renderer, &wall);
            }

        }
    }

    // Draw player
    SDL_Rect p = {
        (int)player.x,
        (int)player.y,
        (int)player.width,
        (int)player.height
    };


    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &p);


    int x = player.x  + (player.width / 2);
    int y = player.y + (player.height / 2);
    int x2 = player.x + player.dx*5 + (player.width / 2);
    int y2 = player.y + player.dy*5 + (player.height / 2);

    SDL_RenderDrawLine(renderer, x, y, x2, y2);

    drawRays2D();

    SDL_RenderPresent(renderer);
}

void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}