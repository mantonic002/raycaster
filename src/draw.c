#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "./constants.h"
#include "./textures.h"
#include "./draw.h"

//----function for rendering the 2D topdown game-----
void render(SDL_Renderer **renderer, Rectangle player, int **map, int *view) {
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
    SDL_RenderClear(*renderer);


    if (*view == 0) {
        //rendering ceeling
        SDL_Rect ceel = {
            0,
            0,
            WINDOW_WIDTH,
            WINDOW_HEIGHT/2
        };

        SDL_SetRenderDrawColor(*renderer, 128, 128, 128, 255);
        SDL_RenderFillRect(*renderer, &ceel);

        //rendering floor
        SDL_Rect floor = {
            0,
            WINDOW_HEIGHT/2,
            WINDOW_WIDTH,
            WINDOW_HEIGHT/2
        };

        SDL_SetRenderDrawColor(*renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(*renderer, &floor);

        drawRays2D(renderer, player, map);
    } else {
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

        int x2 = player.x + player.dx*3;
        int y2 = player.y + player.dy*3;

        SDL_Rect p = {
            player.x - 5,
            player.y - 5,
            10,
            10
        };

        SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);

        SDL_RenderFillRect(*renderer, &p);
        SDL_RenderDrawLine(*renderer, x, y, x2, y2);
    }

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
void drawRays2D(SDL_Renderer **renderer, Rectangle player, int **map) {
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
        else if (ra == 0 || fabs(ra - PI) < EPSILON) { //looking straight left of right
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

        float shade = 0.0f;

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

        //Draw 3D walls
        float ca = fixAng(player.a - ra);

        disT = disT * cos(ca); 
        float lineH = ((BOARD_SIZE * WINDOW_HEIGHT) / disT);

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
                r * LINE_WIDTH + BOARD_SQUARE,
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