//
//  main.c
//  GameOfLife
//
//  Created by Fielding Johnston on 3/11/13.
//
//


#include <iostream>
#include "SDL.h"
#include "SDL_image.h"

using namespace std;

#define DEBUGINFO 0

#define BOARD_GRID_SIZE 16
#define BOARD_SIZE 800

#define CELL_COLOR_ALIVE al_map_rgb(255, 102, 0)
#define CELL_COLOR_DEAD al_map_rgb(0, 0, 0)

int grid[BOARD_SIZE / BOARD_GRID_SIZE][BOARD_SIZE / BOARD_GRID_SIZE];       // Creating a 16 by 16 grid to be used as the game board
int bufferGrid[BOARD_SIZE / BOARD_GRID_SIZE][BOARD_SIZE / BOARD_GRID_SIZE];

int spawn();
int draw();
int update();
int checkNeighbors(int x, int y);
void fillsq(Sint16 x, Sint16 y, Uint16 w, Uint16 h, int color);

// ALLEGRO_BITMAP *image = NULL;
// ALLEGRO_DISPLAY *display = NULL;
// ALLEGRO_KEYBOARD_STATE state;

SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;


int main (int argc, char **argv) {
   

    
    if ( SDL_Init( SDL_INIT_EVERYTHING )){
         printf("SDL_Init: %s\n", SDL_GetError());
    }

    screen = SDL_SetVideoMode( BOARD_SIZE, BOARD_SIZE, 32, SDL_SWSURFACE );
        
    if (!screen) {
        printf("SDL_SetVideoMode: %s\n", SDL_GetError());
    }

    image = IMG_Load( "GameOfLife.app/Contents/Resources/creep.png" );
    
    if(!image) {
        printf("IMG_Load: %s\n", IMG_GetError());
    }
     
    // Initial Draw Tests
    //SDL_BlitSurface( image, NULL, screen, NULL );
    //SDL_Flip( screen );
    //SDL_Delay( 2000 );
    
    //SDL_Rect rect = {50, 50, 16, 16};
    //SDL_FillRect(screen, &rect, 0xFFFFFF);
    //SDL_Flip( screen );
    //SDL_Delay( 2000 );
    
    
    spawn();    // spawn initial cell data
    draw();     // draw the initial board
    
    //while (!al_key_down(&state, ALLEGRO_KEY_ESCAPE))
    while(true)
    {
        update();
        draw();
        // al_get_keyboard_state(&state);
        SDL_Delay( 1000 );
    }
    
    // al_destroy_display(display);
    
    SDL_FreeSurface( image );
    SDL_Quit();
    return 0;
}

int spawn()
{
    // fill the array with random cells
    int rando = 0;
    srand((unsigned)time(NULL));
    
    for (int x = 0; x < BOARD_SIZE / BOARD_GRID_SIZE; x++ ) {
        for (int y = 0; y < BOARD_SIZE / BOARD_GRID_SIZE; y++ ){
            rando = rand() % 2;
            grid[x][y] = rando;
            if(DEBUGINFO){ cout<<"Setting square at "<<x<<","<<y<<" to "<<grid[x][y]<<"\n";}
        }
    }
    
    return 0;
}

int draw()
{
    for (int x = 0; x < BOARD_SIZE / BOARD_GRID_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE / BOARD_GRID_SIZE; y++) {
            if (grid[x][y] == 1){
                
                // Scalable (web-scale lol) images of creeps for the cells
                
                SDL_Rect offset;
                offset.x = x * BOARD_GRID_SIZE;
                offset.y = y * BOARD_GRID_SIZE;
                
                SDL_BlitSurface( image, NULL, screen, &offset );
                
                /*
                al_draw_scaled_bitmap( image,
                    0, 0,                                       // source origin
                    al_get_bitmap_width ( image ),              // source width
                    al_get_bitmap_height ( image),              // source height
                    x * BOARD_GRID_SIZE, y * BOARD_GRID_SIZE,   // target origin
                    8, 8,                                       // target dimensions
                    0                                           // flags
                );
                
                */
                
                // First attempt to use an image for the cells, non-scalable
                /*
                al_draw_bitmap( image,
                    x * BOARD_GRID_SIZE,     // x-coordinate
                    y * BOARD_GRID_SIZE,     // y-coordinate
                    0                        // flags
                    );
                */
                
                
                // original draw rectangle function that was used until I moved to an image
                // al_draw_filled_rectangle(0 + (x * BOARD_GRID_SIZE), 0 + (y * BOARD_GRID_SIZE), BOARD_GRID_SIZE + (x * BOARD_GRID_SIZE), BOARD_GRID_SIZE + (y * BOARD_GRID_SIZE), CELL_COLOR_ALIVE);
                
            } else {
                Sint16 y2 = BOARD_GRID_SIZE * y;
                Sint16 x2 = BOARD_GRID_SIZE * x;
                Uint16 w = BOARD_GRID_SIZE;
                Uint16 h = BOARD_GRID_SIZE;
                SDL_Rect rect = {y2, x2, w, h};
                SDL_FillRect(screen, &rect, 0x000000);
                
                 // al_draw_filled_rectangle(0 + (x * BOARD_GRID_SIZE), 0 + (y * BOARD_GRID_SIZE), BOARD_GRID_SIZE + (x * BOARD_GRID_SIZE), BOARD_GRID_SIZE + (y * BOARD_GRID_SIZE), CELL_COLOR_DEAD);
            }
        }
    }
    
    SDL_Flip( screen );

    return 0;
}

int update()
{
    // Rules:
    // 1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
    // 2. Any live cell with two or three live neighbours lives on to the next generation.
    // 3. Any live cell with more than three live neighbours dies, as if by overcrowding.
    // 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
    
    for ( int x = 0; x < BOARD_SIZE / BOARD_GRID_SIZE; x++ ) {
        for ( int y = 0; y < BOARD_SIZE / BOARD_GRID_SIZE; y++ ){
            int neighbors = checkNeighbors(x, y);
            
            if ( neighbors < 2 && grid[x][y] == 1 ) {
                if(DEBUGINFO){cout<<x<<","<<y<<" has died from under-population!\n";}
                bufferGrid[x][y] = 0;
            } else if ( (neighbors == 2 || neighbors == 3) && grid[x][y] == 1) {
                if(DEBUGINFO){cout<<x<<","<<y<<" has a healthy amount of neighbors and lives on!\n";}
                bufferGrid[x][y] = 1;
            } else if ( neighbors > 3 && grid[x][y] == 1) {
                if(DEBUGINFO){cout<<x<<","<<y<<" has died from overcrowding!\n";}
                bufferGrid[x][y] = 0;
                
            } else if ( neighbors == 3 && grid[x][y] == 0) {
                if(DEBUGINFO){cout<<"A new cell has been born at "<<x<<","<<y<<"!\n";}
                bufferGrid[x][y] = 1;
            }
        }
    }
    
    for ( int x = 0; x < BOARD_SIZE / BOARD_GRID_SIZE; x++) {
        for ( int y = 0; y < BOARD_SIZE / BOARD_GRID_SIZE; y++ ) {
            grid[x][y] = bufferGrid[x][y];
        }
    }
    return 0;
}

int checkNeighbors(int x, int y) {
    int ncount = 0;
    if(DEBUGINFO){cout<<"Checking neighbor cells for " << x << "," << y << ".\n";}
    for ( int a = x - 1; a <= x + 1; a++ ) {
        for ( int b = y - 1; b <= y + 1; b++) {
            if ( grid[a][b] == 1 && a >= 0 && a <= (BOARD_SIZE / BOARD_GRID_SIZE - 1) && b >= 0 && b <= (BOARD_SIZE / BOARD_GRID_SIZE - 1)) {
                ncount++;
            }
        }
    }
    
    if (grid[x][y]) {ncount--;} // subtract self from neighbor count if currently living
    if(DEBUGINFO){cout<<x<<","<<y<<" has "<< ncount << " current neighbor cells.\n";}
    
    return ncount;
}

void fillsq(Sint16 x, Sint16 y, Uint16 w, Uint16 h, int color) {
    SDL_Rect rect = {x,y,w,h};
    SDL_FillRect(screen, &rect, color);
}