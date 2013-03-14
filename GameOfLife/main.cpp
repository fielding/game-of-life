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
#include "SDL_rotozoom.h"

using namespace std;

#define DEBUGINFO 0

#define CELL_SIZE 8      // length and width (in pixels) for the square cells
#define BOARD_SIZE 1600    // Length and width for the square viewing area
#define SCREEN_BPP 8      // Screen bits per-pixels


int grid[BOARD_SIZE / CELL_SIZE][BOARD_SIZE / CELL_SIZE];       // Create a grid based on total board size and each cell size
int bufferGrid[BOARD_SIZE / CELL_SIZE][BOARD_SIZE / CELL_SIZE];

int spawn();
int draw();
int update();
int checkNeighbors( int x, int y );
void fillCell( Sint16 x, Sint16 y, Uint16 w, Uint16 h, int color );

// Surfaces
SDL_Surface *screen = NULL;
SDL_Surface *image = NULL;
SDL_Surface *scaledImage = NULL;

// Event structure
SDL_Event event;

int main ( int argc, char **argv )
{
  bool exit = false;
  
  if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
  {
    printf( "SDL_Init: %s\n", SDL_GetError() );
    return 1;
  }
  
  screen = SDL_SetVideoMode( BOARD_SIZE, BOARD_SIZE, SCREEN_BPP, SDL_SWSURFACE );
        
  if ( screen == NULL )
  {
    printf( "SDL_SetVideoMode: %s\n", SDL_GetError() );
    return 1;
  }
  
  SDL_WM_SetCaption( "Conway's Game of Life", NULL);

  image = IMG_Load( "GameOfLife.app/Contents/Resources/creep.png" );
    
  if ( !image )
  {
    printf( "IMG_Load: %s\n", IMG_GetError() );
  }
  
  scaledImage = rotozoomSurface(image, 0, ( float( CELL_SIZE ) / 16), 0);   // Scale cell image to cell size (16 is based on original png being 16px)
  SDL_FreeSurface( image );   // Finished with image, can free it back up
  
  
  spawn();    // spawn initial cell data
  draw();     // draw the initial board
  
  while ( exit == false )
  {
    update();
    draw();

    while ( SDL_PollEvent( &event ) )
    {
      if ( event.type == SDL_KEYDOWN )   // If a Key was pressed
      {
        switch ( event.key.keysym.sym )
        {
          case SDLK_ESCAPE:   // If key pressed was escape
            exit = true;      // exit the program
            break;
          case SDLK_UP:       // If key pressed was up arrow
            break;
          case SDLK_DOWN:     // If key pressed was down arrow
            break;
          default:            // default for undefined keys
            break;
        }
      } else if ( event.type == SDL_QUIT )
      {
        exit = true;    // exit the program
      }
    }
    SDL_Delay( 500 );   // wait .5 seconds
  }
  
  SDL_FreeSurface( scaledImage );
  SDL_Quit();
  return 0;
}

int spawn()
{
  // fill the array with random cells
  int rando = 0;
  srand(( unsigned )time( NULL ));
    
  for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
  {
    for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
    {
      rando = rand() % 2;
      grid[x][y] = rando;
      if ( DEBUGINFO ){ cout<<"Setting square at "<<x<<","<<y<<" to "<<grid[x][y]<<"\n"; }
    }
  }
  return 0;
}

int draw()
{
  for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
  {
    for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
    {
      if ( grid[x][y] == 1 )
      {
        // Scalable (web-scale lol) images of creeps for the cells
        SDL_Rect offset;
        offset.x = x * CELL_SIZE;
        offset.y = y * CELL_SIZE;
        
        SDL_BlitSurface( scaledImage, NULL, screen, &offset );
      
      } else
      {
        fillCell( CELL_SIZE * x, CELL_SIZE * y, CELL_SIZE, CELL_SIZE, 0x000000 );
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
    
    for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
    {
        for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
        {
            int neighbors = checkNeighbors( x, y );
            
            if ( neighbors < 2 && grid[x][y] == 1 )
            {
                if( DEBUGINFO ){ cout<<x<<","<<y<<" has died from under-population!\n"; }
                bufferGrid[x][y] = 0;
            } else if ( (neighbors == 2 || neighbors == 3) && grid[x][y] == 1)
            {
                if( DEBUGINFO ){ cout<<x<<","<<y<<" has a healthy amount of neighbors and lives on!\n"; }
                bufferGrid[x][y] = 1;
            } else if ( neighbors > 3 && grid[x][y] == 1)
            {
                if( DEBUGINFO ){ cout<<x<<","<<y<<" has died from overcrowding!\n"; }
                bufferGrid[x][y] = 0;
                
            } else if ( neighbors == 3 && grid[x][y] == 0)
            {
                if( DEBUGINFO ){ cout<<"A new cell has been born at "<<x<<","<<y<<"!\n"; }
                bufferGrid[x][y] = 1;
            }
        }
    }
    
    for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
    {
        for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
        {
            grid[x][y] = bufferGrid[x][y];
        }
    }
    return 0;
}

int checkNeighbors( int x, int y )
{
    int ncount = 0;
    if ( DEBUGINFO ){ cout<<"Checking neighbor cells for " << x << "," << y << ".\n"; }
    for ( int a = x - 1; a <= x + 1; a++ )
    {
        for ( int b = y - 1; b <= y + 1; b++)
        {
            if ( grid[a][b] == 1 && a >= 0 && a <= ( BOARD_SIZE / CELL_SIZE - 1 ) && b >= 0 && b <= ( BOARD_SIZE / CELL_SIZE - 1 ) )
            {
                ncount++;
            }
        }
    }
    
    if ( grid[x][y] ) { ncount--; } // subtract self from neighbor count if currently living
    if ( DEBUGINFO ){ cout<<x<<","<<y<<" has "<< ncount << " current neighbor cells.\n"; }
    
    return ncount;
}

void fillCell( Sint16 x, Sint16 y, Uint16 w, Uint16 h, int color )
{
    SDL_Rect rect = { x,y,w,h };
    SDL_FillRect( screen, &rect, color );
}