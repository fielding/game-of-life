//
//  main.c
//  GameOfLife
//
//  Created by Fielding Johnston on 3/11/13.
//
//

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

#define DEBUGINFO 0

#define BOARD_GRID_SIZE 10
#define BOARD_SIZE 800

#define CELL_COLOR_ALIVE al_map_rgb(255, 102, 0)
#define CELL_COLOR_DEAD al_map_rgb(0, 0, 0)

int grid[BOARD_SIZE / BOARD_GRID_SIZE][BOARD_SIZE / BOARD_GRID_SIZE];       // Creating a 16 by 16 grid to be used as the game board
int bufferGrid[BOARD_SIZE / BOARD_GRID_SIZE][BOARD_SIZE / BOARD_GRID_SIZE];

int spawn();
int draw();
int update();
int checkNeighbors(int x, int y);

int main (int argc, char **argv) {
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_KEYBOARD_STATE state;

    
    if (!al_init()){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }
    
    if (!al_init_primitives_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initiliaze al_init_primitives_addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }
    
    if (!al_install_keyboard()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_install_keyboard", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }
    
    display = al_create_display(BOARD_SIZE, BOARD_SIZE);
    
    if (!display) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }
   
    // Initial Draw Tests
    /*
    al_draw_filled_rectangle(0, 0, 10, 10, CELL_COLOR_ALIVE);  // test for drawing alive cell
    al_draw_filled_rectangle(10, 0, 20, 10, CELL_COLOR_DEAD); // test for drawing dead cell
    al_draw_filled_rectangle(10, 10, 20, 20, CELL_COLOR_ALIVE);
    */
    
    spawn();    // spawn initial cell data
    draw();     // draw the initial board
    
    while (!al_key_down(&state, ALLEGRO_KEY_ESCAPE))
    {
        update();
        draw();
        al_get_keyboard_state(&state);
        al_rest(1);
    }
    
    al_destroy_display(display);
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
                al_draw_filled_rectangle(0 + (x * BOARD_GRID_SIZE), 0 + (y * BOARD_GRID_SIZE), BOARD_GRID_SIZE + (x * BOARD_GRID_SIZE), BOARD_GRID_SIZE + (y * BOARD_GRID_SIZE), CELL_COLOR_ALIVE);
            } else {
                al_draw_filled_rectangle(0 + (x * BOARD_GRID_SIZE), 0 + (y * BOARD_GRID_SIZE), BOARD_GRID_SIZE + (x * BOARD_GRID_SIZE), BOARD_GRID_SIZE + (y * BOARD_GRID_SIZE), CELL_COLOR_DEAD);
            }
        }
    }
    al_flip_display();
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